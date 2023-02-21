#pragma once

#include <QTreeView>

namespace Logger
{

class Log;
class LogsModel;
class LogsView : public QTreeView
{
    Q_OBJECT
    Q_PROPERTY(Log* selectedLog READ selectedLog WRITE setSelectedLog NOTIFY selectedLogChanged)

public:
    LogsView(QWidget *parent = nullptr);
    ~LogsView();

    LogsModel *model() const;
    void setModel(LogsModel *newModel);

    Log *selectedLog() const;
    void setSelectedLog(Log *newSelectedLog);

Q_SIGNALS:
    void selectedLogChanged(Log *log);

private:
    void slotSelfClicked(const QModelIndex &index);
    LogsModel *_model;
    Log *m_selectedLog = nullptr;
};

}
