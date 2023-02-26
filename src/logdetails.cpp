#include "Logger/logdetails.h"

#include <QApplication>

#ifdef QT_WIDGETS_LIB

#include "Logger/log.h"

namespace Logger
{

LogDetails::LogDetails(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void LogDetails::setLog(Log *data)
{
    labelType->setText(data->typeString());
    labelTitle->setText(data->message);
    labelFile->setText(data->file);
    labelFunction->setText(data->function);
    labelLine->setText(QString::number(data->line));
}

void LogDetails::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName("LogDetails");
    this->resize(669, 571);
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName("verticalLayout");
    gridLayout = new QGridLayout();
    gridLayout->setObjectName("gridLayout");
    labelDetailMessage_6 = new QLabel(this);
    labelDetailMessage_6->setObjectName("labelDetailMessage_6");
    QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(labelDetailMessage_6->sizePolicy().hasHeightForWidth());
    labelDetailMessage_6->setSizePolicy(sizePolicy);

    gridLayout->addWidget(labelDetailMessage_6, 0, 0, 1, 1);

    labelType = new QLabel(this);
    labelType->setObjectName("labelType");
    labelType->setWordWrap(true);

    gridLayout->addWidget(labelType, 0, 1, 1, 1);

    labelDetailMessage = new QLabel(this);
    labelDetailMessage->setObjectName("labelDetailMessage");
    sizePolicy.setHeightForWidth(labelDetailMessage->sizePolicy().hasHeightForWidth());
    labelDetailMessage->setSizePolicy(sizePolicy);

    gridLayout->addWidget(labelDetailMessage, 1, 0, 1, 1);

    labelTitle = new QLabel(this);
    labelTitle->setObjectName("labelTitle");
    labelTitle->setWordWrap(true);

    gridLayout->addWidget(labelTitle, 1, 1, 1, 1);

    labelDetailMessage_2 = new QLabel(this);
    labelDetailMessage_2->setObjectName("labelDetailMessage_2");
    sizePolicy.setHeightForWidth(labelDetailMessage_2->sizePolicy().hasHeightForWidth());
    labelDetailMessage_2->setSizePolicy(sizePolicy);

    gridLayout->addWidget(labelDetailMessage_2, 2, 0, 1, 1);

    labelBody = new QLabel(this);
    labelBody->setObjectName("labelBody");
    labelBody->setWordWrap(true);

    gridLayout->addWidget(labelBody, 2, 1, 1, 1);

    labelDetailMessage_3 = new QLabel(this);
    labelDetailMessage_3->setObjectName("labelDetailMessage_3");
    sizePolicy.setHeightForWidth(labelDetailMessage_3->sizePolicy().hasHeightForWidth());
    labelDetailMessage_3->setSizePolicy(sizePolicy);

    gridLayout->addWidget(labelDetailMessage_3, 3, 0, 1, 1);

    labelFile = new QLabel(this);
    labelFile->setObjectName("labelFile");
    labelFile->setWordWrap(true);

    gridLayout->addWidget(labelFile, 3, 1, 1, 1);

    labelDetailMessage_4 = new QLabel(this);
    labelDetailMessage_4->setObjectName("labelDetailMessage_4");
    sizePolicy.setHeightForWidth(labelDetailMessage_4->sizePolicy().hasHeightForWidth());
    labelDetailMessage_4->setSizePolicy(sizePolicy);

    gridLayout->addWidget(labelDetailMessage_4, 4, 0, 1, 1);

    labelFunction = new QLabel(this);
    labelFunction->setObjectName("labelFunction");
    labelFunction->setWordWrap(true);

    gridLayout->addWidget(labelFunction, 4, 1, 1, 1);

    labelDetailMessage_5 = new QLabel(this);
    labelDetailMessage_5->setObjectName("labelDetailMessage_5");
    sizePolicy.setHeightForWidth(labelDetailMessage_5->sizePolicy().hasHeightForWidth());
    labelDetailMessage_5->setSizePolicy(sizePolicy);

    gridLayout->addWidget(labelDetailMessage_5, 5, 0, 1, 1);

    labelLine = new QLabel(this);
    labelLine->setObjectName("labelLine");
    labelLine->setWordWrap(true);

    gridLayout->addWidget(labelLine, 5, 1, 1, 1);


    verticalLayout->addLayout(gridLayout);


    retranslateUi();

    QMetaObject::connectSlotsByName(this);
}

void LogDetails::retranslateUi()
{
    this->setWindowTitle(QCoreApplication::translate("LogDetails", "Form", nullptr));
    labelDetailMessage_6->setText(QCoreApplication::translate("LogDetails", "Type:", nullptr));
    labelType->setText(QCoreApplication::translate("LogDetails", "-", nullptr));
    labelDetailMessage->setText(QCoreApplication::translate("LogDetails", "Title:", nullptr));
    labelTitle->setText(QCoreApplication::translate("LogDetails", "-", nullptr));
    labelDetailMessage_2->setText(QCoreApplication::translate("LogDetails", "Body:", nullptr));
    labelBody->setText(QCoreApplication::translate("LogDetails", "-", nullptr));
    labelDetailMessage_3->setText(QCoreApplication::translate("LogDetails", "File:", nullptr));
    labelFile->setText(QCoreApplication::translate("LogDetails", "-", nullptr));
    labelDetailMessage_4->setText(QCoreApplication::translate("LogDetails", "Function:", nullptr));
    labelFunction->setText(QCoreApplication::translate("LogDetails", "-", nullptr));
    labelDetailMessage_5->setText(QCoreApplication::translate("LogDetails", "Line:", nullptr));
    labelLine->setText(QCoreApplication::translate("LogDetails", "-", nullptr));
}

}

#endif
