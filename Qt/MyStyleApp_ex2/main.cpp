#include <QtWidgets>
#include "customstyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget wgt;

    QPushButton* pcmdA = new QPushButton("Button1");
    QPushButton* pcmdB = new QPushButton("Button2");
    QPushButton* pcmdC = new QPushButton("Button3");

    //Change ButtonSize
    pcmdA->setFixedSize(128,128);
    pcmdB->setFixedSize(128,128);
    pcmdC->setFixedSize(128,128);

    //Layout setup
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pcmdA);
    pvbxLayout->addWidget(pcmdB);
    pvbxLayout->addWidget(pcmdC);
    wgt.setLayout(pvbxLayout);

    //CSS Style
    //simple.qss
    //gradient.qss
    QFile file("D:/Qt_Projects/MyStyleApp_ex2/gradient.qss");
    file.open(QFile::ReadOnly);
    QString strCss = QLatin1String(file.readAll());

    a.setStyleSheet(strCss);
    //
    //a.setStyle(new CustomStyle);
    wgt.show();

    return a.exec();
}
