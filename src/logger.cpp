#include <QtCore/QDebug>

#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QMetaMethod>
#include <iostream>


#ifdef QT_WIDGETS_LIB
#include "Logger/showlogdialog.h"
#else
#ifndef LOGGER_NO_CONSOLE
#include "Logger/logconsole.h"
#endif
#endif

#include "Logger/log.h"
#include "Logger/logger.h"
#include "Logger/logsmodel.h"

namespace Logger
{
LogManager instance;
}

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Logger::instance.log(context.file, context.function, context.line, type, msg.toLocal8Bit().data());
}

namespace Logger
{

LogsModel *LogManager::model() const
{
    return _model;
}

LogManager::LogManager(QObject *parent)
    : QObject(parent)
{
    _model = new LogsModel(this);
}

LogManager *LogManager::instance()
{
    static LogManager *instance = 0;

    if (!instance)
        instance = new LogManager;

    return instance;
}

void LogManager::log(const char *fileName,
                 const char *function,
                 int lineNumber,
                 QtMsgType type,
                 const char *templateString,
                 QVariant val0,
                 QVariant val1,
                 QVariant val2,
                 QVariant val3,
                 QVariant val4,
                 QVariant val5,
                 QVariant val6,
                 QVariant val7,
                 QVariant val8,
                 QVariant val9)
{
    QString s(templateString);

    if (val0 != QVariant())
        s = s.arg(val0.toString());
    if (val1 != QVariant())
        s = s.arg(val1.toString());
    if (val2 != QVariant())
        s = s.arg(val2.toString());
    if (val3 != QVariant())
        s = s.arg(val3.toString());
    if (val4 != QVariant())
        s = s.arg(val4.toString());
    if (val5 != QVariant())
        s = s.arg(val5.toString());
    if (val6 != QVariant())
        s = s.arg(val6.toString());
    if (val7 != QVariant())
        s = s.arg(val7.toString());
    if (val8 != QVariant())
        s = s.arg(val8.toString());
    if (val9 != QVariant())
        s = s.arg(val9.toString());

    auto l = new Log;
    l->id = _model->rowCount(QModelIndex()) + 1;
    l->type = static_cast<Logger::LogType>(type);
    l->file = fileName;
    l->function = function;
    l->line = lineNumber;
    l->title = s;
    l->body = QString();
    _model->append(l);

    //    *stream << l->id << " "
    //            << "[" << l->typeString() << "] " << s << Qt::endl;

    if (redirectMessages)
        switch (l->type) {
        case Logger::DebugType:
            qDebug("%s", l->title.toLocal8Bit().data());
            // qDebug("%s %s", l->title.toLocal8Bit().data(), l->file.toLocal8Bit().data());
            break;
        case Logger::InfoType:
            qInfo("%s", l->title.toLocal8Bit().data());
            break;
        case Logger::WarningType:
            qWarning("%s", l->title.toLocal8Bit().data());
            break;
        case Logger::CriticalType:
            qCritical("%s", l->title.toLocal8Bit().data());
            break;
        case Logger::FatalType:
            qFatal("%s", l->title.toLocal8Bit().data());
        }
}

void LogManager::init(Flags f, const QString &path)
{
    if (path == QString())
        _path = qApp->applicationDirPath();
    else
        _path = path;
    qInstallMessageHandler(messageOutput);

    logFile = new QFile(_path + "/" + QDateTime::currentDateTime().toString("ddMMdd-hhmmss.log"));
    logFile->open(QIODevice::WriteOnly | QIODevice::Text);
    stream = new QTextStream(logFile);

    if (f & LogTableView) {
        redirectMessages = false;
#ifdef QT_WIDGETS_LIB
        ShowLogDialog *dialog = new ShowLogDialog;
        dialog->show();
#else
#ifndef LOGGER_NO_CONSOLE
        LogConsole *c = new LogConsole(this);
        c->setTerminalMode(f & TerminalMode);
        c->start();
#endif
#endif
    } else {
        redirectMessages = true;
    }
}

void init(Flags f, const QString &path)
{
    instance.init();
}

}
