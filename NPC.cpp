#include "NPC.h"
#include "Player.h"
#include <iostream>

void Merchant::buyItem(const std::string& goodsName, Player& player)
{
    for (auto& goods : shopGoods)
    {
        if (goods->name == goodsName)
        {
            if (player.gold >= goods->price)
            {
                player.gold -= goods->price;
                player.pickUpItem(std::make_shared<Item>(*goods));
                std::cout << "✅购买成功！获得：" << goodsName << "\n";
                return;
            }
            else
            {
                std::cout << "❌金币不足，无法购买！\n";
                return;
            }
        }
    }
    std::cout << "❌商店没有这件商品！\n";
}

void Merchant::sellItem(const std::string& itemName, Player& player)
{
    auto drop = player.dropItem(itemName);
    if (drop != nullptr)
    {
        int money = drop->price / 2;
        player.gold += money;
        std::cout << "卖出物品，获得" << money << "金币\n";
    }
}