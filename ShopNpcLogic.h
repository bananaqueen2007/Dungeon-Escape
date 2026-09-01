#ifndef SHOPNPCLOGIC_H
#define SHOPNPCLOGIC_H
#include <string>
#include "Player.h"
#include "Room.h"
#include "Merchant.h"

class ShopNpcLogic
{
public:
    static void npcTalk(Room& room, const std::string& npcName);
    static bool openShop(Room& curRoom);
    static bool buyGoods(Player& player, Room& curRoom, const std::string& goodsName);
    static bool sellGoods(Player& player, const std::string& itemName);
};

#endif