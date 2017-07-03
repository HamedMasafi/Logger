#include "logconsole.h"
#include "logger.h"

#include <QModelIndex>
#include <QDebug>
#include <iostream>
#include <iomanip>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <QFile>
#include <QDataStream>
#include <signal.h>


using namespace std;

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_RIGHT 67
#define KEY_PLUS 43
#define KEY_MINUS 45

#define L(s) s.toString().toLatin1().data()

QList<LogConsole*> instances;
void resizeHandler(int sig)
{
    foreach (LogConsole *c, instances)
        c->screenSizeChanged();
}


LogConsole::LogConsole(QObject *parent) : QObject(parent)
{
    instances.append(this);
    _model = Logger::instance();
    for (int i = 0; i < _model->columnCount(); i++)
        headerWidths.insert(i, 10);

    signal(SIGWINCH, resizeHandler);

    QFile f("tabledata.dat");
    if (f.exists()) {
        f.open(QIODevice::ReadOnly);
        QDataStream s(&f);
        QHashIterator<int, int> i(headerWidths);
        while (i.hasNext()) {
            i.next();
            int k;
            int v;
            s >> k;
            s >> v;
            headerWidths.insert(k, v);
        }
        f.close();
    }

    connect(_model, &QAbstractItemModel::rowsInserted,
            [=](const QModelIndex &parent, int first, int last) {
        Q_UNUSED(parent);
        Q_UNUSED(last);
        beginRow = first;
        this->printScreen();

        if (terminalMode) {
            cout << L(_model->data(_model->index(first, 1))) << ": "
                 << L(_model->data(_model->index(first, 2))) << endl;
        }

    });

    struct termios old_tio, new_tio;
    unsigned char c;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    screenSizeChanged();

    beginRow = 0;
    currentColumn = -1;

    CinReader *reader = new CinReader;
    summryMode = terminalMode = false;

    connect(reader, &CinReader::upPressed, [&]() {
        if (beginRow > 0) {
            beginRow--;
            if (summryMode)
                this->printSummry();
            else
                this->printScreen();
        }
    });
    connect(reader, &CinReader::downPressed, [&]() {
        if (beginRow < _model->rowCount() - 1) {
            beginRow++;
            if (summryMode)
                this->printSummry();
            else
                this->printScreen();
        }
    });

    connect(reader, &CinReader::leftPressed, [=]() {
        if (currentColumn > 0) {
            currentColumn--;
            this->printScreen();
        }
    });
    connect(reader, &CinReader::rightPressed, [=]() {
        if (currentColumn < _model->columnCount() - 1) {
            currentColumn++;
            this->printScreen();
        }
    });
    connect(reader, &CinReader::plusPressed, [=]() {
        int sum = 0;
        QHashIterator<int, int> i(headerWidths);
        while (i.hasNext()) {
            i.next();
            sum += i.value();
        }
        if (sum >= width - 1)
            return;
        if (currentColumn >= 0 && currentColumn < _model->columnCount() - 1) {
            headerWidths[currentColumn]++;
            this->printScreen();
        }
    });
    connect(reader, &CinReader::minusPressed, [=]() {
        if (currentColumn >= 0 && currentColumn < _model->columnCount() - 1) {
            headerWidths[currentColumn]--;
            this->printScreen();
        }
    });

    connect(reader, &CinReader::keyPressed, [=](const int &n) {
        if (n == 'q') {
            delete this;
        }
        if (n == 't') {
            terminalMode = !terminalMode;
            this->printScreen();
        }
    });

    connect(reader, &CinReader::returnPressed, [=]() {
        if(terminalMode)
            return;
        summryMode = !summryMode;
        if (summryMode)
            this->printSummry();
        else
            this->printScreen();
    });

    printScreen();
    reader->start();
}

LogConsole::~LogConsole()
{
    QFile f("tabledata.dat");
    f.open(QIODevice::WriteOnly);
    QDataStream s(&f);
    QHashIterator<int, int> i(headerWidths);
    while (i.hasNext()) {
        i.next();
        s << i.key() << i.value();
    }
    f.close();
}

void LogConsole::screenSizeChanged()
{

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    LogConsole::width = w.ws_col;
    LogConsole::height = w.ws_row;

    lines = height - _model->columnCount() - 3;

    printScreen();
}

void LogConsole::printScreen()
{
    cout << "\033[1;1H" << "\033[J";
    if (terminalMode)
        return;

    int from = 0;
    int to = lines;
    int m = _model->rowCount();
    if (m > lines && m - beginRow < lines) {
        from = m - lines;
        to = m;
    }

    cout << "\033[7;21m";
    for (int i = 0; i < headerWidths.size(); i++)
        cout << std::left << setw(headerWidths[i])
             << _model->headerData(i, Qt::Horizontal)
                .toString()
                .toLatin1()
                .data();
    cout << "\033[0m" << endl;
    for (int i = from; i < to; i++)
        printRow(i);

    for (int i = 0; i < width; i++)
        cout << "―";
    cout << endl;

    printSummry();

    cout << "\033[1;1H";
}

void LogConsole::printRow(int row)
{
    if (beginRow == row)
        cout << "\033[7m";
    for (int i = 0; i < headerWidths.size(); i++) {
        QString s = _model->data(_model->index(row, i, QModelIndex()),
                                 Qt::DisplayRole).toString();
        cutText(s, headerWidths.value(i, 10));

        if (beginRow != row && currentColumn == i)
            cout << "\033[1;31m";
        cout << std::left << setw(headerWidths[i]) << s.toLatin1().data();
        if (beginRow != row && currentColumn == i)
            cout << "\033[0m";
    }
    if (beginRow == row)
        cout << "\033[0m";
    cout << endl;
}

void LogConsole::printSummry()
{
    if (summryMode) {
        cout << "\033[1;1H" << "\033[J";
        cout << "\033[7m";
        cout << setw(10) << (beginRow + 1)
             << " of " <<
                setw(10) << _model->rowCount() << endl;
        cout << "\033[0m";
    }


    for (int i = 0; i < _model->columnCount(); i++) {
        cout << setw(10)
             << _model->headerData(i, Qt::Horizontal)
                .toString()
                .toLatin1()
                .data();

        QString buffer = _model->data(_model->index(beginRow, i, QModelIndex()),
                                      Qt::DisplayRole)
                         .toString();
        if (!summryMode)
            cutText(buffer, width - 10);

        cout << buffer.toLatin1().data() << endl;
    }
}

void LogConsole::cutText(QString &s, int len)
{
    if (s.length() > len)
        s = s.left(len);
}

void CinReader::run()
{
    forever
    {
        int i = getchar();
        switch (i) {

        case KEY_UP:
            emit upPressed();
            break;
        case KEY_DOWN:
            emit downPressed();
            break;
        case KEY_RIGHT:
            emit rightPressed();
            break;
        case KEY_LEFT:
            emit leftPressed();
            break;
        case KEY_PLUS:
            emit plusPressed();
            cin.clear();
            break;
        case KEY_MINUS:
            emit minusPressed();
            cin.clear();
            break;

        case 10:
            emit returnPressed();
            break;

        default:
            emit keyPressed(i);
        }
    }
}
