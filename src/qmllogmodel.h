#ifndef QMLLOGMODEL_H
#define QMLLOGMODEL_H

#include <QAbstractListModel>

class QmlLogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Role {
        IdRole = Qt::UserRole + 1
    };

    QmlLogModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
};

#endif // QMLLOGMODEL_H
