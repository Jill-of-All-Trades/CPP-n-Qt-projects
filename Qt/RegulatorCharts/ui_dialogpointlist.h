/********************************************************************************
** Form generated from reading UI file 'dialogpointlist.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPOINTLIST_H
#define UI_DIALOGPOINTLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogPointList
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTableWidget *tableWidget;
    QPushButton *btn_point_add;
    QPushButton *btn_point_delete;
    QPushButton *btn_point_load;
    QPushButton *btn_point_save;
    QWidget *tab_2;
    QLineEdit *le_x_min;
    QLineEdit *le_x_max;
    QLabel *lbl_rangeX;
    QLabel *lbl_axisMin;
    QLabel *lbl_axisMax;
    QLabel *lbl_rangeX_2;
    QLineEdit *le_y_min;
    QLineEdit *le_y_max;

    void setupUi(QDialog *DialogPointList)
    {
        if (DialogPointList->objectName().isEmpty())
            DialogPointList->setObjectName(QString::fromUtf8("DialogPointList"));
        DialogPointList->resize(400, 420);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogPointList->sizePolicy().hasHeightForWidth());
        DialogPointList->setSizePolicy(sizePolicy);
        DialogPointList->setAutoFillBackground(false);
        buttonBox = new QDialogButtonBox(DialogPointList);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 380, 341, 21));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(DialogPointList);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 371, 351));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tableWidget = new QTableWidget(tab);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 361, 281));
        btn_point_add = new QPushButton(tab);
        btn_point_add->setObjectName(QString::fromUtf8("btn_point_add"));
        btn_point_add->setGeometry(QRect(10, 290, 75, 23));
        btn_point_delete = new QPushButton(tab);
        btn_point_delete->setObjectName(QString::fromUtf8("btn_point_delete"));
        btn_point_delete->setGeometry(QRect(90, 290, 75, 23));
        btn_point_load = new QPushButton(tab);
        btn_point_load->setObjectName(QString::fromUtf8("btn_point_load"));
        btn_point_load->setGeometry(QRect(280, 290, 75, 23));
        btn_point_save = new QPushButton(tab);
        btn_point_save->setObjectName(QString::fromUtf8("btn_point_save"));
        btn_point_save->setGeometry(QRect(200, 290, 75, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        le_x_min = new QLineEdit(tab_2);
        le_x_min->setObjectName(QString::fromUtf8("le_x_min"));
        le_x_min->setGeometry(QRect(90, 20, 113, 20));
        le_x_max = new QLineEdit(tab_2);
        le_x_max->setObjectName(QString::fromUtf8("le_x_max"));
        le_x_max->setGeometry(QRect(230, 20, 113, 20));
        lbl_rangeX = new QLabel(tab_2);
        lbl_rangeX->setObjectName(QString::fromUtf8("lbl_rangeX"));
        lbl_rangeX->setGeometry(QRect(10, 20, 71, 16));
        lbl_axisMin = new QLabel(tab_2);
        lbl_axisMin->setObjectName(QString::fromUtf8("lbl_axisMin"));
        lbl_axisMin->setGeometry(QRect(120, 0, 47, 13));
        lbl_axisMin->setAlignment(Qt::AlignCenter);
        lbl_axisMax = new QLabel(tab_2);
        lbl_axisMax->setObjectName(QString::fromUtf8("lbl_axisMax"));
        lbl_axisMax->setGeometry(QRect(260, 0, 47, 13));
        lbl_axisMax->setAlignment(Qt::AlignCenter);
        lbl_rangeX_2 = new QLabel(tab_2);
        lbl_rangeX_2->setObjectName(QString::fromUtf8("lbl_rangeX_2"));
        lbl_rangeX_2->setGeometry(QRect(10, 60, 71, 16));
        le_y_min = new QLineEdit(tab_2);
        le_y_min->setObjectName(QString::fromUtf8("le_y_min"));
        le_y_min->setGeometry(QRect(90, 60, 113, 20));
        le_y_max = new QLineEdit(tab_2);
        le_y_max->setObjectName(QString::fromUtf8("le_y_max"));
        le_y_max->setGeometry(QRect(230, 60, 113, 20));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(DialogPointList);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogPointList, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogPointList, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogPointList);
    } // setupUi

    void retranslateUi(QDialog *DialogPointList)
    {
        DialogPointList->setWindowTitle(QCoreApplication::translate("DialogPointList", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\263\321\200\320\260\321\204\320\270\320\272\320\260", nullptr));
        btn_point_add->setText(QCoreApplication::translate("DialogPointList", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        btn_point_delete->setText(QCoreApplication::translate("DialogPointList", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        btn_point_load->setText(QCoreApplication::translate("DialogPointList", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214...", nullptr));
        btn_point_save->setText(QCoreApplication::translate("DialogPointList", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("DialogPointList", "\320\242\320\276\321\207\320\272\320\270", nullptr));
        le_x_min->setPlaceholderText(QCoreApplication::translate("DialogPointList", "<x min>", nullptr));
        le_x_max->setPlaceholderText(QCoreApplication::translate("DialogPointList", "<x max>", nullptr));
        lbl_rangeX->setText(QCoreApplication::translate("DialogPointList", "\320\223\320\276\321\200\320\270\320\267. \320\276\321\201\321\214", nullptr));
        lbl_axisMin->setText(QCoreApplication::translate("DialogPointList", "Min", nullptr));
        lbl_axisMax->setText(QCoreApplication::translate("DialogPointList", "Max", nullptr));
        lbl_rangeX_2->setText(QCoreApplication::translate("DialogPointList", "\320\222\320\265\321\200\321\202. \320\276\321\201\321\214", nullptr));
        le_y_min->setPlaceholderText(QCoreApplication::translate("DialogPointList", "<y min>", nullptr));
        le_y_max->setPlaceholderText(QCoreApplication::translate("DialogPointList", "<y max>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("DialogPointList", "\320\223\321\200\320\260\321\204\320\270\320\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogPointList: public Ui_DialogPointList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPOINTLIST_H
