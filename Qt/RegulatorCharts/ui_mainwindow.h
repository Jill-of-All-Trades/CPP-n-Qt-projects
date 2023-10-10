/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "dynamicchart.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    DynamicChart *graphicsView;
    QLabel *lbl_mousePos;
    QPushButton *btn_chartSettings;
    QPushButton *btn_calc_path;
    QLabel *lbl_a1;
    QLineEdit *le_a1;
    QLabel *lbl_a2;
    QLineEdit *le_a2;
    QLabel *lbl_a3;
    QLineEdit *le_a3;
    QLineEdit *le_a4;
    QLabel *lbl_a4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(906, 530);
        MainWindow->setAutoFillBackground(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        graphicsView = new DynamicChart(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(50, 30, 521, 361));
        lbl_mousePos = new QLabel(centralWidget);
        lbl_mousePos->setObjectName(QString::fromUtf8("lbl_mousePos"));
        lbl_mousePos->setGeometry(QRect(50, 400, 521, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(12);
        lbl_mousePos->setFont(font);
        lbl_mousePos->setTextFormat(Qt::AutoText);
        btn_chartSettings = new QPushButton(centralWidget);
        btn_chartSettings->setObjectName(QString::fromUtf8("btn_chartSettings"));
        btn_chartSettings->setGeometry(QRect(50, 430, 75, 23));
        btn_calc_path = new QPushButton(centralWidget);
        btn_calc_path->setObjectName(QString::fromUtf8("btn_calc_path"));
        btn_calc_path->setGeometry(QRect(624, 170, 251, 23));
        lbl_a1 = new QLabel(centralWidget);
        lbl_a1->setObjectName(QString::fromUtf8("lbl_a1"));
        lbl_a1->setGeometry(QRect(590, 50, 21, 16));
        lbl_a1->setFont(font);
        le_a1 = new QLineEdit(centralWidget);
        le_a1->setObjectName(QString::fromUtf8("le_a1"));
        le_a1->setEnabled(true);
        le_a1->setGeometry(QRect(620, 50, 261, 20));
        le_a1->setReadOnly(true);
        lbl_a2 = new QLabel(centralWidget);
        lbl_a2->setObjectName(QString::fromUtf8("lbl_a2"));
        lbl_a2->setGeometry(QRect(590, 80, 21, 16));
        lbl_a2->setFont(font);
        le_a2 = new QLineEdit(centralWidget);
        le_a2->setObjectName(QString::fromUtf8("le_a2"));
        le_a2->setEnabled(true);
        le_a2->setGeometry(QRect(620, 80, 261, 20));
        le_a2->setReadOnly(true);
        lbl_a3 = new QLabel(centralWidget);
        lbl_a3->setObjectName(QString::fromUtf8("lbl_a3"));
        lbl_a3->setGeometry(QRect(590, 110, 21, 16));
        lbl_a3->setFont(font);
        le_a3 = new QLineEdit(centralWidget);
        le_a3->setObjectName(QString::fromUtf8("le_a3"));
        le_a3->setEnabled(true);
        le_a3->setGeometry(QRect(620, 110, 261, 20));
        le_a3->setReadOnly(true);
        le_a4 = new QLineEdit(centralWidget);
        le_a4->setObjectName(QString::fromUtf8("le_a4"));
        le_a4->setEnabled(true);
        le_a4->setGeometry(QRect(620, 140, 261, 20));
        le_a4->setReadOnly(true);
        lbl_a4 = new QLabel(centralWidget);
        lbl_a4->setObjectName(QString::fromUtf8("lbl_a4"));
        lbl_a4->setGeometry(QRect(590, 140, 21, 16));
        lbl_a4->setFont(font);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 906, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lbl_mousePos->setText(QCoreApplication::translate("MainWindow", "Mouse position:", nullptr));
        btn_chartSettings->setText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270...", nullptr));
        btn_calc_path->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\207\320\270\321\201\320\273\320\270\321\202\321\214 \320\272\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\321\213", nullptr));
        lbl_a1->setText(QCoreApplication::translate("MainWindow", "a1", nullptr));
        lbl_a2->setText(QCoreApplication::translate("MainWindow", "a2", nullptr));
        lbl_a3->setText(QCoreApplication::translate("MainWindow", "a3", nullptr));
        lbl_a4->setText(QCoreApplication::translate("MainWindow", "a4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
