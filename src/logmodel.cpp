#include "logmodel.h"

#define COL_ID 0
#define COL_Type 1
#define COL_TITLE 2
#define COL_File 3
#define COL_Function 4
#define COL_Line 5

/*
 * Headers:
 *  1: id
 *  2: type
 *  3: body
 *  4: file
 *  5: function
 *  6: line
 */
LogModel::LogModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const
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

int LogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
//    if (!parent.isValid())
//        return 0;

    return dataList.count();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
//    if (!parent.isValid())
//        return 0;

    return 6;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= dataList.size() || index.row() < 0)
        return QVariant("-");

    LogData *d = dataList.at(index.row());
#ifdef QT_QML_LIB
    switch (role) {
    case IdRole:
        return index.row() + 1;
    case TypeRole:
        return d->typeString();
    case TitleRole:
        return d->title;
    case FileRole:
        return d->file;
    case FunctionRole:
        return d->function;
    case LineRole:
        return d->line;
    }
#else
    if (role == Qt::DisplayRole) {

        switch (index.column()) {
        case COL_ID:
            return index.row() + 1;
        case COL_Type:
            return d->typeString();
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
#endif
    return QVariant();
}

QHash<int, QByteArray> LogModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {TitleRole, "title"},
        {TypeRole, "type"},
        {FileRole, "file"},
        {FunctionRole, "func"},
        {LineRole, "line"}
    };
}

void LogModel::append(LogModel::LogData *row)
{
    beginInsertRows(QModelIndex(), dataList.count(), dataList.count());
    dataList.append(row);
    endInsertRows();
}

LogModel::LogData *LogModel::row(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    if (index.row() >= dataList.size() || index.row() < 0)
        return nullptr;

    return dataList.at(index.row());
}

QString LogModel::LogData::typeString() const
{
    switch (type) {
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

    return "";
}
