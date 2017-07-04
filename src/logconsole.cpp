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
#define VLINE "│"
#define HLINE "―"
#define HLINE_TOP "┴"
#define CORNER_BL "└"

QList<LogConsole *> instances;
void resizeHandler(int sig)
{
    foreach (LogConsole *c, instances)
        c->screenSizeChanged();
}

LogConsole::LogConsole(QObject *parent) : QObject(parent)
{
    beginColumn = 0;
    printLines = true;
    status = "t=Terminal mode    Enter=Show details    +/-=Change column size";
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

        if (terminalMode) {
            for (int i = first; i < last; i++)
                this->printRow(i);
        } else {
            beginRow = first;
            this->printScreen();
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
        if(terminalMode)
            return;

        if (beginRow > 0) {
            beginRow--;
            if (summryMode)
                this->printSummry();
            else
                this->printScreen();
        }
    });
    connect(reader, &CinReader::downPressed, [&]() {
        if(terminalMode)
            return;

        if (beginRow < _model->rowCount() - 1) {
            beginRow++;
            if (summryMode)
                this->printSummry();
            else
                this->printScreen();
        }
    });

    connect(reader, &CinReader::leftPressed, [=]() {
        if(terminalMode)
            return;

        if (beginColumn > 0) {
            beginColumn--;
            status = QString::number(beginColumn);
            this->printScreen();
        }
    });
    connect(reader, &CinReader::rightPressed, [=]() {
        if(terminalMode)
            return;

        if (beginColumn < width) {
            beginColumn++;
            status = QString::number(beginColumn);
            this->printScreen();
        }
    });

    connect(reader, &CinReader::tabPressed, [=](bool shift) {
        if (shift){
            if (--currentColumn <= -1)
                currentColumn = headerWidths.size() - 1;
        } else {
            if (++currentColumn == headerWidths.size())
                currentColumn = 0;
        }

        this->printScreen();
    });

    connect(reader, &CinReader::plusPressed, [=]() {
        if(terminalMode)
            return;

//        int sum = 0;
//        QHashIterator<int, int> i(headerWidths);
//        while (i.hasNext()) {
//            i.next();
//            sum += i.value();
//        }
//        if (sum >= width - 1)
//            return;
        if (currentColumn >= 0 && currentColumn < _model->columnCount() - 1) {
            headerWidths[currentColumn]++;
            this->printScreen();
        }
    });
    connect(reader, &CinReader::minusPressed, [=]() {
        if(terminalMode)
            return;

        if (currentColumn >= 0 && currentColumn < _model->columnCount() - 1 && headerWidths[currentColumn] > 0) {
            headerWidths[currentColumn]--;
            this->printScreen();
        }
    });

    connect(reader, &CinReader::keyPressed, [=](const int &n) {
        if (n == 'q') {
            delete this;
//            reader->quit();
        }
        if (n == '|') {
            printLines = !printLines;
            this->printScreen();
        }
        if (n == '0') {
            headerWidths[0] = 0;
        }
        if (n == 't') {
            terminalMode = !terminalMode;
            this->clearScreen();
            if (terminalMode) {
                int from = 0;
                if (_model->rowCount() > lines)
                    from = _model->rowCount() - lines;
                for (int i = from; i < _model->rowCount(); i++)
                    this->printRow(i);
            } else {
                this->printScreen();
            }
        }
    });

    connect(reader, &CinReader::returnPressed, [=]() {
        if (terminalMode)
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
    clearScreen();
    if (terminalMode)
        return;

    int from = 0;
    int to = lines;
    int m = _model->rowCount();
    if (m > lines && m - beginRow < lines) {
        from = m - lines;
        to = m;
    }

//    const char *l = "";
//    if (printLines)
//        l = VLINE;

//    inverseColorBg();
//    int rowLen = 0;
//    for (int i = 0; i < headerWidths.size(); i++) {
//        cout << l << std::left << setw(headerWidths[i])
//             << _model->headerData(i, Qt::Horizontal)
//                    .toString()
//                    .toLatin1()
//                    .data();
//        rowLen += headerWidths[i] + (printLines ? 1 : 0);
//    }
//    cout << l;
//    if (rowLen < width)
//        for (int i = rowLen; i < width - (printLines ? 1 : 0); i++)
//            cout << " ";
//    restoreTxetColor();
//    cout << endl;

    printRow(-1);
    for (int i = from; i < to; i++)
        printRow(i);

//    if (printLines) {
//        for (int i = 0; i < headerWidths.size(); i++) {
//            for (int n = 0; n < headerWidths[i]; n++)
//                cout << HLINE;
//            cout << HLINE_TOP;
//        }
//    } else {
//        for (int i = 0; i < width; i++)
//            cout << HLINE;
//    }
//    cout << endl;
    printRow(-2);

    printSummry();
    inverseColorBg();
    cout << status.toLatin1().data();
    for (int i = status.length(); i < width; i++)
        cout << " ";
    restoreTxetColor();
}

void LogConsole::printType(QString t, bool printColon)
{

    if (t == "Debug")
        setTextColor(Blue);
    else if (t == "Info")
        setTextColor(Cyan);
    else if (t == "Warning")
        setTextColor(Yellow);
    else if (t == "Error" || t == "Fatal")
        setTextColor(Red);
    else
        setTextColor(Magenta);

    cout << t.toLatin1().data();
    if (printColon)
            cout << ": ";
    restoreTxetColor();
}

//void LogConsole::printRow(int row)
//{
//    if (terminalMode) {
//        QString t = _model->data(_model->index(row, 1)).toString();

//        printType(t);

//        cout << L(_model->data(_model->index(row, 2))) << endl;
//        return;
//    }
//    const char *l = "";
//    if (printLines)
//        l = VLINE;

//    int rowLen = 0;
//    if (beginRow == row)
//        inverseColorBg();

//    for (int i = 0; i < headerWidths.size(); i++) {
//        QString s = _model->data(_model->index(row, i, QModelIndex()),
//                                 Qt::DisplayRole).toString();

//        int fieldLen = headerWidths.value(i, 10);

//        if (rowLen + fieldLen > width - (printLines ? 1 : 0))
//            fieldLen = width - rowLen - (printLines ? 1 : 0);

//        cutText(s, fieldLen);

//        cout << l;
//        if (beginRow != row && currentColumn == i)
//            setTextColor(Yellow, Black, true);

//        cout << std::left << setw(fieldLen) << s.toLatin1().data();

//        if (beginRow != row && currentColumn == i)
//            restoreTxetColor();

//        rowLen += fieldLen + (printLines ? 1 : 0);

//        if (rowLen >= width)
//            break;
//    }

//    cout << l;
//    if (beginRow == row) {
//        if (rowLen < width)
//            for (int i = rowLen; i < width - (printLines ? 1 : 0); i++)
//                cout << " ";
//        restoreTxetColor();
//    }
//    cout << endl;
//}

void LogConsole::printRow(int row)
{
    if (terminalMode) {
        QString t = _model->data(_model->index(row, 1)).toString();

        printType(t);

        cout << L(_model->data(_model->index(row, 2))) << endl;
        return;
    }

    int skipped = 0;
    bool isHeader = row == -1;
    const char *l = "";
    if (printLines)
        l = (row == -2) ? HLINE_TOP : VLINE;

    int rowLen = 0;
    if (beginRow == row || isHeader)
        inverseColorBg();

    for (int i = 0; i < headerWidths.size(); i++) {
        skipped += headerWidths[i];
        if (skipped <= beginColumn)
            continue;

        QString s = "";

        if(row == -1){
            s = _model->headerData(i, Qt::Horizontal).toString();
        } else if (row != -2){
            s = _model->data(_model->index(row, i, QModelIndex()),
                             Qt::DisplayRole).toString();
        }

        int fieldLen = headerWidths.value(i, 10);

        if (rowLen + fieldLen > width - (printLines ? 1 : 0))
            fieldLen = width - rowLen - (printLines ? 1 : 0);

        if (skipped > beginColumn && skipped - fieldLen < beginColumn) {
            s = s.remove(0, fieldLen - (skipped - beginColumn));
            fieldLen = skipped - beginColumn;
        } else {
            if (i)
                cout << l;
        }

        cutText(s, fieldLen);

        if (row >= 0 && beginRow != row && currentColumn == i)
            setTextColor(Yellow, Black, true);

        if (row == -2)
            for(int ic = 0; ic < fieldLen; ic++)
                cout << HLINE;
        else
            cout << std::left << setw(fieldLen) << s.toLatin1().data();

        if (row >= 0 && beginRow != row && currentColumn == i)
            restoreTxetColor();

        rowLen += fieldLen + (printLines ? 1 : 0);

        if (rowLen >= width)
            break;
    }

    if (beginRow == row || row < 0) {
        if (rowLen < width)
            for (int i = rowLen; i < width - (printLines ? 1 : 0); i++)
                cout << (row == -2 ? HLINE : " ");
        restoreTxetColor();
    }
    cout << endl;
}

void LogConsole::printSummry()
{
    if (summryMode) {
        clearScreen();
        //        inverseColorBg();
        inverseColorBg();
        cout << setw(10) << (beginRow + 1) << " of " << setw(10)
             << _model->rowCount() << endl;
        restoreTxetColor();
    }

    for (int i = 0; i < _model->columnCount(); i++) {
        cout << setw(10)
             << _model->headerData(i, Qt::Horizontal)
                    .toString()
                    .toLatin1()
                    .data();

        QString buffer = _model->data(_model->index(beginRow, i, QModelIndex()),
                                      Qt::DisplayRole).toString();
        if (!summryMode)
            cutText(buffer, width - 10);

        if (i == 1)
            printType(buffer, false);
        else
            cout << buffer.toLatin1().data();

        cout << endl;
    }
}

QString LogConsole::fill(const char *s, int len)
{
    QString st;
    for(int i = 0; i < len; i++)
        st.append(HLINE);
    return st;
}

void LogConsole::cutText(QString &s, int len)
{
    if (s.length() > len)
        s = s.left(len);
}

void LogConsole::clearScreen()
{
    cout << "\033[1;1H"
         << "\033[J";
}

void LogConsole::moveCursor(int x, int y)
{
    cout << QString("\033[<%1><%1>").arg(x).arg(y).toLatin1().data();
}

void LogConsole::setTextColor(Color text, Color bg, bool bright)
{
    /*
    black        30         40
    red          31         41
    green        32         42
    yellow       33         43
    blue         34         44
    magenta      35         45
    cyan         36         46
    white        37         47
    */
    cout << QString("\033[%1;%2%3m")
                .arg((int)text + 30)
                .arg((int)bg + 40)
                .arg(bright ? ";1" : "")
                .toLatin1()
                .data();
}

void LogConsole::restoreTxetColor()
{
    cout << "\033[0m";
}

void LogConsole::inverseColorBg()
{
    cout << "\033[7m";
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

        case 9:
            emit tabPressed(false);
            break;

        case 90:
            emit tabPressed(true);
            break;

        default:
//            cout << i << endl;
            emit keyPressed(i);
        }
    }
}
