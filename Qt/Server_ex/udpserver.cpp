#include "udpserver.h"
#include <QtWidgets>
#include <QUdpSocket>

UdpServer::UdpServer(QWidget* pwgt) : QTextEdit (pwgt)
{
    setWindowTitle("UdpServer");

    m_pudp = new QUdpSocket(this);

    QTimer* ptimer = new QTimer(this);
    ptimer->setInterval(500);
    ptimer->start();
    connect(ptimer,SIGNAL(timeout()),SLOT(slotSendDatagram()));
}

void UdpServer::slotSendDatagram()
{
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_13);
    QDateTime dt = QDateTime::currentDateTime();
    append("Sent:" + dt.toString());
    out << dt;
    m_pudp->writeDatagram(baDatagram, QHostAddress::LocalHost, 2424);
}


//CLIENT
UdpClient::UdpClient(QWidget* pwgt) : QTextEdit (pwgt)
{
    setWindowTitle("UdpClient");

    m_pudp = new QUdpSocket(this);
    m_pudp->bind(2424);
    connect(m_pudp,SIGNAL(readyRead()),SLOT(slotProcessDatagrams()));
}

void UdpClient::slotProcessDatagrams()
{
    QByteArray baDatagram;

    do {
        baDatagram.resize(m_pudp->pendingDatagramSize());
        m_pudp->readDatagram(baDatagram.data(),baDatagram.size());
    } while (m_pudp->hasPendingDatagrams());

    QDateTime dateTime;
    QDataStream in(&baDatagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_13);
    in>>dateTime;
    append("Received:"+dateTime.toString());
}
