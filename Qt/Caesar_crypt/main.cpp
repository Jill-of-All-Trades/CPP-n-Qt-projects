#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include "caesarcipherdevice.h"

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    QByteArray ba = "plaintext";
    QBuffer buf;
    buf.open(QIODevice::WriteOnly);
    CaesarCipherDevice encrypt;
    encrypt.setKey(3);
    encrypt.setBaseDevice(&buf);
    encrypt.open(buf.openMode());
    encrypt.write(ba);
    qDebug() << buf.data();

    CaesarCipherDevice decrypt;
    decrypt.setKey(3);
    decrypt.setBaseDevice(&buf);
    buf.open(QIODevice::ReadOnly);
    decrypt.open(buf.openMode());
    qDebug() << decrypt.readAll();

    return 0;
    //return a.exec();
}
