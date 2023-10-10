#ifndef PLAYERINFOREADER_H
#define PLAYERINFOREADER_H

class QIODevice;
#include <QXmlStreamReader>
#include "player_structs.h"

class PlayerInfoReader
{
public:
    PlayerInfoReader(QIODevice *);
    inline const PlayerInfo& result() const { return m_pinfo; }

    enum Token {
        T_Invalid = -1,
        T_PlayerInfo,                                   /* root tag */
        T_Player,                                       /* in PlayerInfo */
        T_Name, T_Password, T_Inventory, T_Location,    /* in Player */
        T_Position,                                     /* in Location */
        T_InvItem                                       /* in Inventory */
    };

    static Token tokenByName(const QStringRef &r);

    bool read();
    bool readPlayerInfo();
    Player readPlayer();
    QList<Item> readInventory();

private:
    QXmlStreamReader reader;
    PlayerInfo m_pinfo;
};

#endif // PLAYERINFOREADER_H
