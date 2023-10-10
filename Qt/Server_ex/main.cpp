#include <QtWidgets>
#include "myserver.h"
#include "myclient.h"
#include "udpserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ///TCP SERVER
    /*
    MyServer server(2323);
    server.show();

    MyClient client("localhost",2323);
    client.show();
    */

    ///UDP SERVER
    UdpServer server;
    server.show();

    UdpClient client;
    client.show();

    return a.exec();
}
