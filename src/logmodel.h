#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/qglobal.h>

class LogModel : public QAbstractTableModel
{
    Q_OBJECT

    struct LogData{
        int id;
        int type;
        QString body;
        QString file;
        QString function;
        int line;
    };
    QList<LogData*> dataList;

public:
    explicit LogModel(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
};

#endif // LOGMODEL_H
