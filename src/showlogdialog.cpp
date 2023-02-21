#include "Logger/showlogdialog.h"
#include "Logger/log.h"

#ifdef QT_WIDGETS_LIB

#include "Logger/logger.h"
#include "Logger/logsmodel.h"

#include <QtCore/QSortFilterProxyModel>

ShowLogDialog::ShowLogDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(Logger::instance.model());
    treeView->setModel(proxyModel);

    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1);
    proxyModel->sort(0, Qt::AscendingOrder);
}

void ShowLogDialog::changeEvent(QEvent *e)
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

void ShowLogDialog::on_lineEdit_textChanged(const QString &s)
{
    proxyModel->setFilterWildcard("*" + s + "*");
}

void ShowLogDialog::on_treeView_clicked(const QModelIndex &index)
{
    auto i = proxyModel->mapToSource(index);
    auto data = Logger::instance.model()->row(i);

    if (!data)
        return;

    labelType->setText(data->typeString());
    labelTitle->setText(data->title);
    labelBody->setText(data->body);
    labelFile->setText(data->file);
    labelFunction->setText(data->function);
    labelLine->setText(QString::number(data->line));
}

#endif
