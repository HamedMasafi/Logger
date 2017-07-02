#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore/qglobal.h>
#include <QtCore/QAbstractTableModel>

#include <QVariant>

#define K_TRACE_DEBUG  qDebug() << "Trace at: " <<  __FILE__ << __FUNCTION__ << " #" << __LINE__
#define K_LOG(type, ...) Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, type, __VA_ARGS__)

#define K_TRACE(...)        Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtDebugMsg, __VA_ARGS__)
#define K_INFORMATION(...)  Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtInfoMsg, __VA_ARGS__)
#define K_WARNING(...)      Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtWarningMsg, __VA_ARGS__)
#define K_ERROR(...)        Logger::instance()->log(__FILE__, __FUNCTION__, __LINE__, QtCriticalMsg, __VA_ARGS__)

class LogModel;
class Logger : public QAbstractTableModel
{
    Q_OBJECT

public:
    Logger(QObject *parent = 0);

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


    static Logger *instance();
    void log(const char *fileName, const char *function, int lineNumber,
             QtMsgType type,
             const char *templateString,
             QVariant val0 = QVariant(),
             QVariant val1 = QVariant(),
             QVariant val2 = QVariant(),
             QVariant val3 = QVariant(),
             QVariant val4 = QVariant(),
             QVariant val5 = QVariant(),
             QVariant val6 = QVariant(),
             QVariant val7 = QVariant(),
             QVariant val8 = QVariant(),
             QVariant val9 = QVariant());

    void installMessageHangler();
    void showDialog();

private:
    struct LogData{
        int id;
        QtMsgType type;
        QString title;
        QString body;
        QString file;
        QString function;
        int line;
    };
    QList<LogData*> dataList;

};

#endif // LOGGER_H
