#ifndef SHOPNPCLOGIC_H
#define SHOPNPCLOGIC_H

#include <string>
#include "Player.h"
#include "Room.h"
#include "NPC.h"

class ShopNpcLogic
{
public:
    //talk和npc对话
    static void npcTalk(Room& room, const std::string& npcName);
    //打开商店
    static bool openShop(Room& curRoom);
    //购买商品
    static bool buyGoods(Player& player, Room& curRoom, const std::string& goodsName);
    //卖出物品
    static bool sellGoods(Player& player, const std::string& itemName);
};

#endif