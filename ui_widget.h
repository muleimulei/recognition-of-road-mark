/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "mylabel.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QTabWidget *tabWidget;
    QWidget *tab1;
    MyLabel *player;
    QWidget *tab2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QWidget *tab;
    MyLabel *framediff;
    QWidget *tab4;
    QLabel *labelall;
    QPushButton *captureButton;
    QPushButton *chooseFile;
    QLabel *currentrate;
    QLabel *rate;
    QLabel *totalrate;
    QPushButton *start;
    QPushButton *stop;
    QLabel *graydiff;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(821, 496);
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 831, 391));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tab1 = new QWidget();
        tab1->setObjectName(QStringLiteral("tab1"));
        player = new MyLabel(tab1);
        player->setObjectName(QStringLiteral("player"));
        player->setGeometry(QRect(0, 0, 831, 371));
        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QStringLiteral("tab2"));
        scrollArea = new QScrollArea(tab2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 821, 371));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 819, 369));
        scrollAreaWidgetContents->setMinimumSize(QSize(0, 0));
        gridLayoutWidget = new QWidget(scrollAreaWidgetContents);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(130, 110, 511, 171));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea->setWidget(scrollAreaWidgetContents);
        tabWidget->addTab(tab2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        framediff = new MyLabel(tab);
        framediff->setObjectName(QStringLiteral("framediff"));
        framediff->setGeometry(QRect(280, 140, 155, 65));
        tabWidget->addTab(tab, QString());
        tab4 = new QWidget();
        tab4->setObjectName(QStringLiteral("tab4"));
        labelall = new QLabel(tab4);
        labelall->setObjectName(QStringLiteral("labelall"));
        labelall->setGeometry(QRect(0, 0, 821, 371));
        tabWidget->addTab(tab4, QString());
        captureButton = new QPushButton(Widget);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(80, 460, 80, 20));
        chooseFile = new QPushButton(Widget);
        chooseFile->setObjectName(QStringLiteral("chooseFile"));
        chooseFile->setGeometry(QRect(210, 460, 80, 20));
        currentrate = new QLabel(Widget);
        currentrate->setObjectName(QStringLiteral("currentrate"));
        currentrate->setGeometry(QRect(180, 390, 121, 31));
        rate = new QLabel(Widget);
        rate->setObjectName(QStringLiteral("rate"));
        rate->setGeometry(QRect(330, 390, 121, 31));
        totalrate = new QLabel(Widget);
        totalrate->setObjectName(QStringLiteral("totalrate"));
        totalrate->setGeometry(QRect(450, 390, 121, 31));
        start = new QPushButton(Widget);
        start->setObjectName(QStringLiteral("start"));
        start->setGeometry(QRect(350, 460, 80, 20));
        stop = new QPushButton(Widget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(490, 460, 80, 20));
        graydiff = new QLabel(Widget);
        graydiff->setObjectName(QStringLiteral("graydiff"));
        graydiff->setGeometry(QRect(580, 390, 121, 31));

        retranslateUi(Widget);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", nullptr));
        player->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("Widget", "\350\247\206\351\242\221", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("Widget", "\346\210\252\345\233\276", nullptr));
        framediff->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Widget", "\345\270\247\351\227\264\345\267\256", nullptr));
        labelall->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab4), QApplication::translate("Widget", "\347\201\260\345\272\246\345\233\276", nullptr));
        captureButton->setText(QApplication::translate("Widget", "\346\210\252\345\261\217", nullptr));
        chooseFile->setText(QApplication::translate("Widget", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        currentrate->setText(QApplication::translate("Widget", "\345\275\223\345\211\215\345\270\247\346\225\260\357\274\232", nullptr));
        rate->setText(QApplication::translate("Widget", "\345\270\247\347\216\207\357\274\232", nullptr));
        totalrate->setText(QApplication::translate("Widget", "\346\200\273\345\270\247\346\225\260\357\274\232", nullptr));
        start->setText(QApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        stop->setText(QApplication::translate("Widget", "\346\232\202\345\201\234", nullptr));
        graydiff->setText(QApplication::translate("Widget", "\347\201\260\345\272\246\345\267\256\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
