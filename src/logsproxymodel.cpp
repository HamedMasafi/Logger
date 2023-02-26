#include "Logger/logsproxymodel.h"
#include "Logger/log.h"
#include "Logger/logger.h"
#include "Logger/logsmodel.h"

namespace Logger {

LogsProxyModel::LogsProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , _parentModel(instance.model())
{
    setSourceModel(_parentModel);
}

LogsProxyModel::LogsProxyModel(LogsModel *model, QObject *parent)
    : QSortFilterProxyModel(parent)
    , _parentModel(model)
{
    setSourceModel(_parentModel);
}

QString LogsProxyModel::filterText() const
{
    return _filterText;
}

void LogsProxyModel::setFilterText(const QString &newFilterText)
{
    _filterText = newFilterText;
    invalidateRowsFilter();
}

bool LogsProxyModel::showDebug() const
{
    return _showDebug;
}

void LogsProxyModel::setShowDebug(bool newShowDebug)
{
    _showDebug = newShowDebug;
    invalidateRowsFilter();
}

bool LogsProxyModel::showInformation() const
{
    return _showInformation;
}

void LogsProxyModel::setShowInformation(bool newShowInformation)
{
    _showInformation = newShowInformation;
    invalidateRowsFilter();
}

bool LogsProxyModel::showWarning() const
{
    return _showWarning;
}

void LogsProxyModel::setShowWarning(bool newShowWarning)
{
    _showWarning = newShowWarning;
    invalidateRowsFilter();
}

bool LogsProxyModel::showCritical() const
{
    return _showCritical;
}

void LogsProxyModel::setShowCritical(bool newShowCritical)
{
    _showCritical = newShowCritical;
    invalidateRowsFilter();
}

bool LogsProxyModel::showFatal() const
{
    return _showFatal;
}

void LogsProxyModel::setShowFatal(bool newShowFatal)
{
    _showFatal = newShowFatal;
    invalidateRowsFilter();
}

bool LogsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

    auto row = _parentModel->row(source_row);

    if (!row)
        return false;

    if (!_showDebug && row->type == Logger::DebugType)
        return false;

    if (!_showWarning && row->type == Logger::WarningType)
        return false;

    if (!_showCritical && row->type == Logger::CriticalType)
        return false;

    if (!_showFatal && row->type == Logger::FatalType)
        return false;

    if (!_showInformation && row->type == Logger::InfoType)
        return false;


    if (_filterText.size() && !row->message.contains(_filterText) && !row->category.contains(_filterText))
        return false;

    return true;
}

}
