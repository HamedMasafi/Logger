#include "logsproxymodel.h"

LogsProxyModel::LogsProxyModel(LogModel *model, QObject *parent) : QSortFilterProxyModel(parent)
{

}

QString LogsProxyModel::filterText() const
{
    return _filterText;
}

void LogsProxyModel::setFilterText(const QString &newFilterText)
{
    _filterText = newFilterText;
}

bool LogsProxyModel::showTrace() const
{
    return _showTrace;
}

void LogsProxyModel::setShowTrace(bool newShowTrace)
{
    _showTrace = newShowTrace;
}

bool LogsProxyModel::showDebug() const
{
    return _showDebug;
}

void LogsProxyModel::setShowDebug(bool newShowDebug)
{
    _showDebug = newShowDebug;
}

bool LogsProxyModel::showInformation() const
{
    return _showInformation;
}

void LogsProxyModel::setShowInformation(bool newShowInformation)
{
    _showInformation = newShowInformation;
}

bool LogsProxyModel::showWarning() const
{
    return _showWarning;
}

void LogsProxyModel::setShowWarning(bool newShowWarning)
{
    _showWarning = newShowWarning;
}

bool LogsProxyModel::showError() const
{
    return _showError;
}

void LogsProxyModel::setShowError(bool newShowError)
{
    _showError = newShowError;
}
