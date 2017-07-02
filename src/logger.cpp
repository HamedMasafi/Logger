#include <QtCore/QDebug>

#include <QMetaMethod>
#include <iostream>

#ifdef QT_WIDGETS_LIB
#   include "showlogdialog.h"
#else
#   include "logconsole.h"
#endif

#include "logger.h"

#define COL_ID 0
#define COL_Type 1
#define COL_TITLE 2
#define COL_File 3
#define COL_Function 4
#define COL_Line 5

void messageOutput(QtMsgType type, const QMessageLogContext &context,
                   const QString &msg)
{
    Logger::instance()->log(context.file, context.function, context.line, type,
                            msg.toLocal8Bit().data());
}

Logger::Logger(QObject *parent) : QAbstractTableModel(parent)
{
    qInstallMessageHandler(messageOutput);
}

QVariant Logger::headerData(int section, Qt::Orientation orientation,
                            int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case COL_ID:
            return tr("ID");
        case COL_Type:
            return tr("Type");
        case COL_TITLE:
            return tr("Message");
        case COL_File:
            return tr("File");
        case COL_Function:
            return tr("Function");
        case COL_Line:
            return tr("Line");
        }
    }
    return QVariant();
}

int Logger::rowCount(const QModelIndex &parent) const
{
    //    if (!parent.isValid())
    //        return 0;

    return dataList.count();
}

int Logger::columnCount(const QModelIndex &parent) const
{
    //    if (!parent.isValid())
    //        return 0;

    return 6;
}

QVariant Logger::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= dataList.size() || index.row() < 0)
        return QVariant("-");

    if (role == Qt::DisplayRole) {
        LogData *d = dataList.at(index.row());

        switch (index.column()) {
        case COL_ID:
            return index.row() + 1;
        case COL_Type: {
            switch (d->type) {
            case QtDebugMsg:
                return "Debug";
                break;
            case QtInfoMsg:
                return "Info";
                break;
            case QtWarningMsg:
                return "Warning";
                break;
            case QtCriticalMsg:
                return "Error";
                break;
            case QtFatalMsg:
                return "Fatal";
            }
        }
        case COL_TITLE:
            return d->title;
        case COL_File:
            return d->file;
        case COL_Function:
            return d->function;
        case COL_Line:
            return d->line;
        }
    }
    return QVariant();
}

bool Logger::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool Logger::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool Logger::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool Logger::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
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

    beginInsertRows(QModelIndex(), dataList.count(), dataList.count() + 1);

    LogData *l = new LogData;
    l->id = dataList.count();
    l->type = type;
    l->file = fileName;
    l->function = function;
    l->line = lineNumber;
    l->title = s;
    l->body = QString::null;
    dataList.append(l);

//    switch (l->type) {
//    case QtDebugMsg:
//        qDebug("%s", l->title.toLocal8Bit().data());
//        break;
//    case QtInfoMsg:
//        qInfo("%s", l->title.toLocal8Bit().data());
//        break;
//    case QtWarningMsg:
//        qWarning("%s", l->title.toLocal8Bit().data());
//        break;
//    case QtCriticalMsg:
//        qCritical("%s", l->title.toLocal8Bit().data());
//        break;
//    case QtFatalMsg:
//        qFatal("%s", l->title.toLocal8Bit().data());
//    }
    endInsertRows();
}

void Logger::showDialog()
{
#ifdef QT_WIDGETS_LIB
    showLogDialog *dialog = new showLogDialog;
    dialog->show();
#else
    LogConsole *c = new LogConsole(this);
    Q_UNUSED(c);
#endif
}
