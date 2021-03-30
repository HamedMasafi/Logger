#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/qglobal.h>

class LogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    struct LogData {
        int id;
        QtMsgType type;
        QString title;
        QString body;
        QString file;
        QString function;
        int line;

        QString typeString() const;
    };
#ifdef QT_QML_LIB
    enum Role {
        IdRole = Qt::UserRole + 1,
        TypeRole,
        TitleRole,
        FileRole,
        FunctionRole,
        LineRole
    };

#endif
    explicit LogModel(QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void append(LogData *row);
    LogData *row(const QModelIndex &index) const;

private:

    QList<LogData*> dataList;
};

#endif // LOGMODEL_H
