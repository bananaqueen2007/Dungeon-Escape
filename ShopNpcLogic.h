#ifndef SHOPNPCLOGIC_H
#define SHOPNPCLOGIC_H

#include <string>
#include "Player.h"
#include "Room.h"
#include "Merchant.h"
using namespace std;

class ShopNpcLogic
{
public:
    static void npcTalk(Room& room, Player& player, const string& npcName);
    static bool openShop(Room& curRoom);
    static bool buyGoods(Player& player, Room& curRoom, const string& goodsName);
    static bool sellGoods(Player& player, const string& itemName);
};

#endif