#pragma once

#include <QtCore/qglobal.h>
#include <QtCore/QAbstractTableModel>

#include <QVariant>

#define K_TRACE_DEBUG  qDebug() << "Trace at: " <<  __FILE__ << __FUNCTION__ << " #" << __LINE__
#define K_LOG(type, ...) Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, type, __VA_ARGS__)

#define K_TRACE(...)        Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtDebugMsg, __VA_ARGS__)
#define K_DEBUG(...)        Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtDebugMsg, __VA_ARGS__)
#define K_INFORMATION(...)  Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtInfoMsg, __VA_ARGS__)
#define K_WARNING(...)      Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtWarningMsg, __VA_ARGS__)
#define K_ERROR(...)        Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtCriticalMsg, __VA_ARGS__)

class LogModel;
class QFile;
class QTextStream;
class Logger : public QObject
{
    Q_OBJECT
    QFile *logFile;
    QTextStream *stream;
    bool redirectMessages;
    LogModel *_model;
    QString _path;

public:
    enum Flag{
        None = 0,

        LogTableView = 1,
        TerminalLines = 2,
        TerminalMode = 4
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    Logger(QObject *parent = 0);

    static Logger *instance();
    void log(const char *fileName, const char *function, int lineNumber,
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

    void init(Logger::Flags f = None, const QString &path = QString());
    LogModel *model() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::Flags)
