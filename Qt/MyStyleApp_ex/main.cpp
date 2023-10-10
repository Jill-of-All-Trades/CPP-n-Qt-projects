#include <QApplication>
#include "myapplication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyApplication myApp;

    myApp.show();

    return a.exec();
}
