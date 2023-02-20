#include <QSortFilterProxyModel>

#pragma once

class LogModel;
class LogsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    LogsProxyModel(LogModel *model = nullptr, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QString filterText() const;
    void setFilterText(const QString &newFilterText);

    Q_REQUIRED_RESULT bool showTrace() const;
    void setShowTrace(bool newShowTrace);

    Q_REQUIRED_RESULT bool showDebug() const;
    void setShowDebug(bool newShowDebug);

    Q_REQUIRED_RESULT bool showInformation() const;
    void setShowInformation(bool newShowInformation);

    Q_REQUIRED_RESULT bool showWarning() const;
    void setShowWarning(bool newShowWarning);

    Q_REQUIRED_RESULT bool showError() const;
    void setShowError(bool newShowError);

private:
    QString _filterText;
    bool _showTrace{true};
    bool _showDebug{true};
    bool _showInformation{true};
    bool _showWarning{true};
    bool _showError{true};

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};
