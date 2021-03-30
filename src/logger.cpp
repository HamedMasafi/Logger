#include <QtCore/QDebug>

#include <QFile>
#include <QDateTime>
#include <QMetaMethod>
#include <iostream>
#include <QApplication>

#ifdef QT_WIDGETS_LIB
#include "showlogdialog.h"
#else
#   ifndef LOGGER_NO_CONSOLE
#       include "logconsole.h"
#   endif
#endif

#include "logger.h"
#include "logmodel.h"

void messageOutput(QtMsgType type, const QMessageLogContext &context,
                   const QString &msg)
{
    Logger::instance()->log(context.file, context.function, context.line, type,
                            msg.toLocal8Bit().data());
}

LogModel *Logger::model() const
{
    return _model;
}

Logger::Logger(QObject *parent) : QObject(parent)
{
    _model = new LogModel(this);
}

Logger *Logger::instance()
{
    static Logger *instance = 0;

    if (!instance)
        instance = new Logger;

    return instance;
}

void Logger::log(const char *fileName, const char *function, int lineNumber,
                 QtMsgType type, const char *templateString, QVariant val0,
                 QVariant val1, QVariant val2, QVariant val3, QVariant val4,
                 QVariant val5, QVariant val6, QVariant val7, QVariant val8,
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

    auto l = new LogModel::LogData;
    l->id = _model->rowCount(QModelIndex()) + 1;
    l->type = type;
    l->file = fileName;
    l->function = function;
    l->line = lineNumber;
    l->title = s;
    l->body = QString();
    _model->append(l);

    *stream << l->id << " "
            << "[" << l->typeString() << "] " << s << Qt::endl;

    if (redirectMessages)
        switch (l->type) {
        case QtDebugMsg:
            qDebug("%s", l->title.toLocal8Bit().data());
            // qDebug("%s %s", l->title.toLocal8Bit().data(), l->file.toLocal8Bit().data());
            break;
        case QtInfoMsg:
            qInfo("%s", l->title.toLocal8Bit().data());
            break;
        case QtWarningMsg:
            qWarning("%s", l->title.toLocal8Bit().data());
            break;
        case QtCriticalMsg:
            qCritical("%s", l->title.toLocal8Bit().data());
            break;
        case QtFatalMsg:
            qFatal("%s", l->title.toLocal8Bit().data());
        }
}

void Logger::init(Flags f, const QString &path)
{
    if (path == QString())
        _path = qApp->applicationDirPath();
    else
        _path = path;
    qInstallMessageHandler(messageOutput);

    logFile
        = new QFile(_path + "/" + QDateTime::currentDateTime().toString("ddMMdd-hhmmss.log"));
    logFile->open(QIODevice::WriteOnly | QIODevice::Text);
    stream = new QTextStream(logFile);

    if (f & LogTableView) {
        redirectMessages = false;
#ifdef QT_WIDGETS_LIB
        ShowLogDialog *dialog = new ShowLogDialog;
        dialog->show();
#else
#   ifndef LOGGER_NO_CONSOLE
        LogConsole *c = new LogConsole(this);
        c->setTerminalMode(f & TerminalMode);
        c->start();
#   endif
#endif
    } else {
        redirectMessages = true;
    }
}

