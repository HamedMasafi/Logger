#ifndef LOGCONSOLE_H
#define LOGCONSOLE_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>

#include <QHash>
#include <QThread>

class LogModel;
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
    void numPressed(const int &num);
};

class LogConsole : public QObject
{
    Q_OBJECT
    LogModel *_model;

    int currentRow;
    int beginRow;
    int lines;

    int beginColumn;
    int currentColumn;

    int width;
    int height;

    QHash<int, int> headerWidths;
    QHash<int, bool> headerVisible;
    CinReader *reader;
    bool summryMode;
    bool _terminalMode;

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

    void start();
    bool terminalMode() const;
    void setTerminalMode(bool terminalMode);

private:
    void printScreen();
    void printRow(int row, bool scrollbar = false);
    void printSummry(bool buffering = true);
    QString fill(const char *s, int len);
    void cutText(QString &s, int len);
    void clearScreen();
    void setUnderline();
    void moveCursor(int x, int y);
    void setTextColor(Color text, Color bg = Black, bool bright = true);
    void restoreTxetColor();
    void inverseColorBg();

    void printType(QString t, bool printColon = true);
    
signals:

public slots:
};

#endif // LOGCONSOLE_H
