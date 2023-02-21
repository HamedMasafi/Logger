#pragma once

#include <QSortFilterProxyModel>

namespace Logger {

class LogsModel;
class LogsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    LogsProxyModel(QObject *parent = nullptr);
    LogsProxyModel(LogsModel *model, QObject *parent = nullptr);

    Q_REQUIRED_RESULT QString filterText() const;
    void setFilterText(const QString &newFilterText);

    Q_REQUIRED_RESULT bool showDebug() const;
    void setShowDebug(bool newShowDebug);

    Q_REQUIRED_RESULT bool showInformation() const;
    void setShowInformation(bool newShowInformation);

    Q_REQUIRED_RESULT bool showWarning() const;
    void setShowWarning(bool newShowWarning);

    Q_REQUIRED_RESULT bool showError() const;
    void setShowError(bool newShowError);

    Q_REQUIRED_RESULT bool showCritical() const;
    void setShowCritical(bool newShowCritical);

    Q_REQUIRED_RESULT bool showFatal() const;
    void setShowFatal(bool newShowFatal);

private:
    QString _filterText;
    bool _showDebug{true};
    bool _showWarning{true};
    bool _showCritical{true};
    bool _showInformation{true};
    bool _showFatal{true};

protected:
    LogsModel *_parentModel;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

}
