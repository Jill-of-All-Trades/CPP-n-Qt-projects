#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QTextEdit>
#include <QUdpSocket>

class UdpServer : public QTextEdit
{
    Q_OBJECT
private:
    QUdpSocket* m_pudp;
public:
    UdpServer(QWidget* pwgt = nullptr);
private slots:
    void slotSendDatagram();
};


//Client
class UdpClient : public QTextEdit {
    Q_OBJECT
private:
    QUdpSocket* m_pudp;
public:
    UdpClient(QWidget* pwgt = nullptr);
private slots:
    void slotProcessDatagrams();
};

#endif // UDPSERVER_H
