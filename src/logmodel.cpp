#include "logmodel.h"

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
        case 0:
            return tr("ID");
        case 1:
            return tr("Type");
        case 2:
            return tr("Body");
        case 3:
            return tr("File");
        case 4:
            return tr("Function");
        case 5:
            return tr("Line");
        }
    }
    return QVariant();
}

int LogModel::rowCount(const QModelIndex &parent) const
{
//    if (!parent.isValid())
//        return 0;

    return dataList.count() + 10;
}

int LogModel::columnCount(const QModelIndex &parent) const
{
//    if (!parent.isValid())
//        return 0;

    return 6;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

//    if (index.row() >= dataList.size() || index.row() < 0)
//        return QVariant();

    if (role == Qt::DisplayRole) {

        if (index.column() == 0)
            return "pair.first";
        else if (index.column() == 1)
            return "pair.second";
    }
    return QVariant();
}

bool LogModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool LogModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool LogModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool LogModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}
