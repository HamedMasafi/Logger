#pragma once

#include <QObject>
#include <QString>

namespace Logger
{

Q_NAMESPACE

enum LogType {
    DebugType = QtDebugMsg,
    WarningType = QtWarningMsg,
    CriticalType = QtCriticalMsg,
    FatalType = QtFatalMsg,
    InfoType = QtInfoMsg,
    SystemType = QtSystemMsg
};
Q_ENUM_NS(LogType)

class Log
{
public:
    Log();

    Log(int id, LogType type, QString title, QString body, QString file, QString function, QString category, int line);
    int id;
    LogType type;
    QString title;
    QString body;
    QString file;
    QString function;
    QString category;
    int line;

    QString typeString() const;
};

}
