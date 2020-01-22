#ifndef SHOWLOGDIALOG_H
#define SHOWLOGDIALOG_H

#ifdef QT_WIDGETS_LIB

#include "ui_showlogdialog.h"

class QSortFilterProxyModel;
class ShowLogDialog : public QDialog, private Ui::showLogDialog
{
    Q_OBJECT
    QSortFilterProxyModel *proxyModel;

public:
    explicit ShowLogDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_treeView_clicked(const QModelIndex &index);
};

#endif

#endif // SHOWLOGDIALOG_H
