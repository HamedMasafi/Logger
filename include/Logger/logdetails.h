#pragma once

#include "qwidget.h"

#include <QGridLayout>
#include <QLabel>
#ifdef QT_WIDGETS_LIB

namespace Logger {
class Log;

class LogDetails : public QWidget
{
    Q_OBJECT

public:
    explicit LogDetails(QWidget *parent = nullptr);

    void setupUi();
    void retranslateUi();

public Q_SLOTS:
    void setLog(Log *log);

private:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *labelDetailMessage_6;
    QLabel *labelType;
    QLabel *labelDetailMessage;
    QLabel *labelTitle;
    QLabel *labelDetailMessage_2;
    QLabel *labelBody;
    QLabel *labelDetailMessage_3;
    QLabel *labelFile;
    QLabel *labelDetailMessage_4;
    QLabel *labelFunction;
    QLabel *labelDetailMessage_5;
    QLabel *labelLine;
};

}

#endif
