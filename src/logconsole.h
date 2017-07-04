#ifndef LOGCONSOLE_H
#define LOGCONSOLE_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QHash>
#include <QThread>

class Logger;
class CinReader : public QThread
{
    Q_OBJECT

    // QThread interface
protected:
    void run();

signals:
    void keyPressed(const int &key);
    void upPressed();
    void downPressed();
    void leftPressed();
    void rightPressed();
    void plusPressed();
    void minusPressed();
    void returnPressed();
    void tabPressed(bool shift);
};

class LogConsole : public QObject
{
    Q_OBJECT
    Logger *_model;
    int beginRow;
    int beginColumn;
    int lines;
    QHash<int, int> headerWidths;
    int currentColumn;
    CinReader *reader;
    bool summryMode;
    bool terminalMode;
    int width;
    int height;
    QString status;
    bool printLines;

    enum Color{
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

public:
    explicit LogConsole(QObject *parent = nullptr);
    virtual ~LogConsole();
    void screenSizeChanged();

private:
    void printScreen();
    void printRow(int row);
    void printSummry();
    QString fill(const char *s, int len);
    void cutText(QString &s, int len);
    void clearScreen();
    void moveCursor(int x, int y);
    void setTextColor(Color text, Color bg = Black, bool bright = true);
    void restoreTxetColor();
    void inverseColorBg();

    void printType(QString t, bool printColon = true);
    
signals:

public slots:
};

#endif // LOGCONSOLE_H
