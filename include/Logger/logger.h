#pragma once

#include <QtCore/qglobal.h>
#include <QtCore/QAbstractTableModel>

#include <QVariant>

#define K_TRACE_DEBUG  qDebug() << "Trace at: " <<  __FILE__ << __FUNCTION__ << " #" << __LINE__
#define K_LOG(type, ...) Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, type, __VA_ARGS__)

#define K_TRACE(...)        Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, QtDebugMsg, __VA_ARGS__)

#define K_DEBUG(...)        Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, QtDebugMsg, __VA_ARGS__)
#define K_WARNING(...)      Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, QtWarningMsg, __VA_ARGS__)
#define K_CRITICAL(...)     Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, QtCriticalMsg, __VA_ARGS__)
#define K_FATAL(...)        Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, QtFatalMsg, __VA_ARGS__)
#define K_INFORMATION(...)  Logger::instance.log(__FILE__, __FUNCTION__, __LINE__, QtInfoMsg, __VA_ARGS__)

class QFile;
class QTextStream;

namespace Logger {

class LogsModel;
enum Flag{
    None = 0,

    LogTableView = 1,
    TerminalLines = 2,
    TerminalMode = 4
};
Q_DECLARE_FLAGS(Flags, Flag)
Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::Flags)

class LogManager : public QObject
{
    Q_OBJECT
    QFile *logFile;
    QTextStream *stream;
    bool redirectMessages;
    LogsModel *_model;
    QString _path;

public:

    LogManager(QObject *parent = 0);

    static LogManager *instance();
    void log(const char *fileName, const char *function, int lineNumber, const char *category,
             QtMsgType type,
             const char *templateString,
             QVariant val0 = QVariant(),
             QVariant val1 = QVariant(),
             QVariant val2 = QVariant(),
             QVariant val3 = QVariant(),
             QVariant val4 = QVariant(),
             QVariant val5 = QVariant(),
             QVariant val6 = QVariant(),
             QVariant val7 = QVariant(),
             QVariant val8 = QVariant(),
             QVariant val9 = QVariant());

    void init(Flags f = None, const QString &path = QString());
    LogsModel *model() const;
};

void init(Flags f = None, const QString &path = QString());
extern LogManager instance;

}

