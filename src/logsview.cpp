#include "Logger/logsview.h"
#include "Logger/logger.h"
#include "Logger/log.h"
#include "Logger/logsmodel.h"

#include <QSettings>
#include <QHeaderView>

namespace Logger
{
LogsView::LogsView(QWidget *parent)
    : QTreeView(parent)
{
    setSelectionBehavior(SelectRows);
    setSelectionMode(SingleSelection);

    setModel(instance.model());

    connect(this, &QTreeView::clicked, this, &LogsView::slotSelfClicked);

    QSettings s;
    s.beginGroup("Logger");
    if (s.contains("LogsViewGeometry"))
        header()->restoreState(s.value("LogsViewGeometry").toByteArray());
}

LogsView::~LogsView()
{
    QSettings s;
    s.beginGroup("Logger");
    s.setValue("LogsViewGeometry", header()->saveState());
    s.sync();
}

LogsModel *LogsView::model() const
{
    return _model;
}

void LogsView::setModel(LogsModel *newModel)
{
    _model = newModel;
    QTreeView::setModel(newModel);
}

void LogsView::slotSelfClicked(const QModelIndex &index)
{
    auto row = _model->row(index);
    setSelectedLog(row);
}

Log *LogsView::selectedLog() const
{
    return m_selectedLog;
}

void LogsView::setSelectedLog(Log *newSelectedLog)
{
    if (m_selectedLog == newSelectedLog)
        return;
    m_selectedLog = newSelectedLog;
    Q_EMIT selectedLogChanged(newSelectedLog);
}

}
