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
};

class LogConsole : public QObject
{
    Q_OBJECT
    Logger *_model;
    int beginRow;
    int lines;
    QHash<int, int> headerWidths;
    int width;
    int height;
    int currentColumn;
    CinReader *reader;

public:
    explicit LogConsole(QObject *parent = nullptr);

private:
    void printScreen();
    void printRow(int i);
    void printSummry();
    void cutText(QString &s, int len);

signals:

public slots:
};

#endif // LOGCONSOLE_H
