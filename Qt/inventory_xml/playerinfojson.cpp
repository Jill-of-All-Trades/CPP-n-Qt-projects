#include "playerinfojson.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

PlayerInfoJSON::PlayerInfoJSON()
{

}

QByteArray PlayerInfoJSON::writePlayerInfo(const PlayerInfo &pinfo) const
{
   QJsonDocument doc = QJsonDocument::fromVariant(toVariant(pinfo));
   return doc.toJson();
}

//read ByteArray
PlayerInfo PlayerInfoJSON::readPlayerInfo(const QByteArray &ba) const
{
    QJsonDocument doc = QJsonDocument::fromJson(ba);
    if(doc.isEmpty() || !doc.isArray()) return PlayerInfo();
    return readPlayerInfo(doc.array());
}

//read QJsonArray
PlayerInfo PlayerInfoJSON::readPlayerInfo(const QJsonArray &array) const
{
    PlayerInfo pinfo;
    foreach(QJsonValue value, array)
        pinfo.players << readPlayer(value.toObject());
    return pinfo;
}

//read QJsonObeject
Player PlayerInfoJSON::readPlayer(const QJsonObject &object) const
{
    Player player;
    player.name = object.value("name").toString();
    player.password = object.value("password").toString();
    player.experience = object.value("experience").toDouble();
    player.hitPoints = object.value("hitpoints").toDouble();
    player.location = object.value("location").toString();
    QVariantMap positionMap = object.value("position").toVariant().toMap();
    player.position = QPoint(positionMap["x"].toInt(), positionMap["y"].toInt());
    player.inventory = readInventory(object.value("inventory").toArray());
    return player;
}

QVariant PlayerInfoJSON::toVariant(const PlayerInfo &pinfo) const
{
    QVariantList players;
    foreach(const Player &p, pinfo.players) players << toVariant(p);
    return players;
}

QVariant PlayerInfoJSON::toVariant(const Player &player) const
{
    QVariantMap map;
    map["name"]         = player.name;
    map["password"]     = player.password;
    map["experience"]   = player.experience;
    map["hitpoints"]    = player.hitPoints;
    map["location"]     = player.location;
    map["position"]     = QVariantMap({ {"x", player.position.x()},
                                        {"y", player.position.y()}});
    map["inventory"]    = toVariant(player.inventory);
    return map;
}

QVariant PlayerInfoJSON::toVariant(const QList<Item> &items) const
{
    QVariantList list;
    foreach(const Item &item, items) list << toVariant(item);
    return list;
}

QVariant PlayerInfoJSON::toVariant(const Item &item) const
{
    QVariantMap map;
    map["type"] = (int)item.type;
    map["subtype"] = item.subType;
    map["durability"] = item.durability;
    return map;
}

QList<Item> PlayerInfoJSON::readInventory(const QJsonArray &array) const
{
    QList<Item> inventory;
    foreach(QJsonValue value, array)
        inventory << readItem(value.toObject());
    return inventory;
}

Item PlayerInfoJSON::readItem(const QJsonObject &object) const
{
    Item item;
    item.type = (Item::Type)object.value("type").toDouble();
    item.subType = object.value("subtype").toString();
    item.durability = object.value("durability").toDouble();
    return item;
}
