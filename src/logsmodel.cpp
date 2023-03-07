#include "Logger/logsmodel.h"
#include "Logger/log.h"

#include <QThread>

#define COL_ID 0
#define COL_Type 1
#define COL_Message 2
#define COL_File 3
#define COL_Function 4
#define COL_Line 5
#define COL_Category 6

/*
 * Headers:
 *  1: id
 *  2: type
 *  3: body
 *  4: file
 *  5: function
 *  6: line
 */

namespace Logger
{

LogsModel::LogsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant LogsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case COL_ID:
            return tr("ID");
        case COL_Type:
            return tr("Type");
        case COL_Message:
            return tr("Message");
        case COL_File:
            return tr("File");
        case COL_Function:
            return tr("Function");
        case COL_Line:
            return tr("Line");
        case COL_Category:
            return tr("Category");
        }
    }
    return QVariant();
}

int LogsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //    if (!parent.isValid())
    //        return 0;

    return dataList.count();
}

int LogsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //    if (!parent.isValid())
    //        return 0;

    return 7;
}

QVariant LogsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= dataList.size() || index.row() < 0)
        return QVariant("-");

    auto d = dataList.at(index.row());
#ifdef QT_QML_LIB
    switch (role) {
    case IdRole:
        return index.row() + 1;
    case TypeRole:
        return d->type;
    case TypeStringRole:
        return d->typeString();
    case TitleRole:
        return d->title;
    case FileRole:
        return d->file;
    case FunctionRole:
        return d->function;
    case LineRole:
        return d->line;
    case CategoryRole:
        return d->category;
    }
#else
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case COL_ID:
            return index.row() + 1;
        case COL_Type:
            return d->typeString();
        case COL_Message:
            return d->message;
        case COL_File:
            return d->file;
        case COL_Function:
            return d->function;
        case COL_Line:
            return d->line;
        case COL_Category:
            return d->category;
        }
    }
#endif
    return QVariant();
}

#ifdef QT_QML_LIB
QHash<int, QByteArray> LogModel::roleNames() const
{
    return {{IdRole, "id"},
            {MessageRole, "message"},
            {TypeRole, "type"},
            {TypeStringRole, "typeString"},
            {FileRole, "file"},
            {FunctionRole, "func"},
            {LineRole, "line"},
            {CategoryRole, "category"}};
}
#endif

void LogsModel::append(Log *row)
{
    if (QThread::currentThread() == thread())
        beginInsertRows(QModelIndex(), dataList.count(), dataList.count());

    dataList.append(row);

    if (QThread::currentThread() == thread())
        endInsertRows();
}

Log *LogsModel::row(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    if (index.row() >= dataList.size() || index.row() < 0)
        return nullptr;

    return dataList.at(index.row());
}

Log *LogsModel::row(int index) const
{
    return dataList.at(index);
}

}
