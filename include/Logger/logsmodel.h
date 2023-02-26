#pragma once

#include <QtCore/QAbstractTableModel>
#include <QtCore/qglobal.h>

namespace Logger
{

class Log;
class LogsModel : public QAbstractTableModel
{
    Q_OBJECT

public:

#ifdef QT_QML_LIB
    enum Role { IdRole = Qt::UserRole + 1, TypeRole, TypeStringRole, MessageRole, FileRole, FunctionRole, LineRole, CategoryRole };

    QHash<int, QByteArray> roleNames() const override;
#endif
    explicit LogsModel(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void append(Log *row);
    Log *row(const QModelIndex &index) const;
    Log *row(int index) const;

private:
    QList<Log *> dataList;
};
}
