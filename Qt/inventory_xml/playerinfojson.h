#ifndef PLAYERINFOJSON_H
#define PLAYERINFOJSON_H

#include "player_structs.h"

class PlayerInfoJSON
{
public:
    PlayerInfoJSON();
    QByteArray writePlayerInfo(const PlayerInfo &pinfo) const;
    PlayerInfo readPlayerInfo(const QByteArray &ba) const;
    PlayerInfo readPlayerInfo(const QJsonArray &array) const;
    Player readPlayer(const QJsonObject &object) const;
protected:
    QVariant toVariant(const PlayerInfo &pinfo) const;
    QVariant toVariant(const Player &player) const;
    QVariant toVariant(const QList<Item> &items) const;
    QVariant toVariant(const Item &item) const;
    QList<Item> readInventory(const QJsonArray &array) const;
    Item readItem(const QJsonObject & object) const;
};


#endif // PLAYERINFOJSON_H
