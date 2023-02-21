#pragma once

#include <QFile>
#include <QString>
#include <QTextStream>

namespace Logger {
class LogsModel;

class Manager
{
public:
    Manager();

private:
    QFile *logFile;
    QTextStream *stream;
    bool redirectMessages;
    LogsModel *_model;
    QString _path;
};

}
