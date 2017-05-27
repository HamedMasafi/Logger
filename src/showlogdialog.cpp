#include "showlogdialog.h"

#ifdef QT_WIDGETS_LIB

#include "logger.h"
#include "logmodel.h"

#include <QtCore/QSortFilterProxyModel>

showLogDialog::showLogDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(Logger::instance());
    treeView->setModel(proxyModel);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    proxyModel->sort(0, Qt::AscendingOrder);
}

void showLogDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void showLogDialog::on_lineEdit_textChanged(const QString &s)
{
    proxyModel->setFilterWildcard("*" + s + "*");
}

#endif
