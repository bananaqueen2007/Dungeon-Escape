#include "ShopNpcLogic.h"
#include"ChestStory.h"
#include <iostream>
#include <algorithm>
#include <windows.h>

#define BLUE  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9)
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7)

void ShopNpcLogic::npcTalk(Room& room, Player& player, const std::string& npcName)
{
    for (auto& npc : room.npcs)
    {
        if (npc->name == npcName)
        {
            npc->talk();
            if (npcName == "鼠鼠大王")
            {
                if (!player.hasBoneKey)
                {
                    bool getGift = ChestStory::meetMouseKing(player);
                    if (getGift)
                    {
                        npc->triggeredOnce = true;
                    }
                }
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
            BLUE;
            mer->showShop();
            WHITE;
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
        int price = 0;
        if (itemName == "蝙蝠的翅膀") price = 10;
        else if (itemName == "夜明砂") price = 15;
        else if (itemName == "生锈的刀") price = 9;
        else if (itemName == "蟾蜍粘液") price = 12;
        else if (itemName == "珍贵的蟾蜍粘液") price = 30;
        else if (itemName == "灰扑扑的晶核") price = 20;
        else if (itemName == "散发着诡异光芒的晶核") price = 50;
        else if (itemName == "破损的蛛丝") price = 15;
        else if (itemName == "完好的蛛丝") price = 40;
        else price = 30;

        int sellPrice = dropPtr->stackCount * price;
        player.gold += sellPrice;
        std::cout << "卖出物品，获得" << sellPrice << "金币。" << std::endl;
        return true;
    }
    return false;
}