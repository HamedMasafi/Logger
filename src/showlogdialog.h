#ifndef SHOWLOGDIALOG_H
#define SHOWLOGDIALOG_H

#ifdef QT_WIDGETS_LIB

#include "ui_showlogdialog.h"

class QSortFilterProxyModel;
class showLogDialog : public QDialog, private Ui::showLogDialog
{
    Q_OBJECT
    QSortFilterProxyModel *proxyModel;

public:
    explicit showLogDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
};

#endif

#endif // SHOWLOGDIALOG_H
