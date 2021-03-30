#include "qmllogmodel.h"

QmlLogModel::QmlLogModel(QObject *parent) : QAbstractListModel(parent)
{

}

int QmlLogModel::rowCount(const QModelIndex &parent) const
{

}

QVariant QmlLogModel::data(const QModelIndex &index, int role) const
{

}

QHash<int, QByteArray> QmlLogModel::roleNames() const
{
    return {
        {IdRole, "id"}
    };
}
