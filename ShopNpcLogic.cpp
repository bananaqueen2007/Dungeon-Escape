#include "ShopNpcLogic.h"
#include <iostream>

void ShopNpcLogic::npcTalk(Room& room, const std::string& npcName)
{
    for (auto& npc : room.npcs)
    {
        if (npc->name == npcName)
        {
            npc->talk();
            //鼠鼠大王特殊交互
            if (npcName == "鼠鼠大王")
            {
                ChestStory::meetMouseKing(*(&room));
            }
            return;
        }
    }
    std::cout << "房间内找不到这个NPC！" << std::endl;
}

bool ShopNpcLogic::openShop(Room& curRoom)
{
    for (auto& npc : curRoom.npcs)
    {
        Merchant* mer = dynamic_cast<Merchant*>(npc.get());
        if (mer != nullptr)
        {
            mer->showShop();
            return true;
        }
    }
    std::cout << "这里没有黑市商人，无法打开商店！" << std::endl;
    return false;
}

bool ShopNpcLogic::buyGoods(Player& player, Room& curRoom, const std::string& goodsName)
{
    for (auto& npc : curRoom.npcs)
    {
        Merchant* mer = dynamic_cast<Merchant*>(npc.get());
        if (mer)
        {
            mer->buyItem(goodsName, player);
            return true;
        }
    }
    std::cout << "当前位置没有商人！" << std::endl;
    return false;
}

bool ShopNpcLogic::sellGoods(Player& player, const std::string& itemName)
{
    auto dropPtr = player.dropItem(itemName);
    if (dropPtr != nullptr)
    {
        int sellPrice = dropPtr->stackCount * 30;
        player.gold += sellPrice;
        std::cout << "卖出物品，获得" << sellPrice << "金币。" << std::endl;
        return true;
    }
    return false;
}