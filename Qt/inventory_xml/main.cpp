#include "playerinforeader.h"
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QDir dir = QDir::current();
    QFile file(dir.filePath("inventory.xml"));
    file.open(QFile::ReadOnly | QFile::Text);
    PlayerInfoReader reader(&file);
    bool playerOk = reader.read();
    PlayerInfo playerInfo;
    if(playerOk) playerInfo = reader.result();

    if(!playerInfo.players.isEmpty()) {
        qDebug() << "Count:" << playerInfo.players.count();
        qDebug() << "Size of inventory:" <<
                    playerInfo.players.first().inventory.size();
        qDebug() << "Inventory item:"
                 << playerInfo.players.first().inventory[0].type
                 << playerInfo.players.first().inventory[0].subType;
        qDebug() << "Room:" << playerInfo.players.first().location
                 << playerInfo.players.first().position;
    }

    return 0;
}
