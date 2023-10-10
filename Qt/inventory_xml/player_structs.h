#ifndef PLAYER_STRUCTS_H
#define PLAYER_STRUCTS_H

#include <QString>
#include <QList>
//Structs
struct Item {
    enum Type { Weapon, Armor, Gem, Book, Other } type;
    QString subType;
    int durability;
};

struct Player {
    QString name;
    QString password;
    int experience;
    int hitPoints;
    QList<Item> inventory;
    QString location;
    QPoint position;
};

struct PlayerInfo {
    QList<Player> players;
};

#endif // PLAYER_STRUCTS_H
