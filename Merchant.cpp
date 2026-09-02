#include "Merchant.h"
#include <iostream>

Merchant::Merchant(std::string name, std::string talk) :Npc(name, talk) {}

void Merchant::showShop()
{
    std::cout << "\n====商人商店====" << std::endl;
    for (auto& g : shopGoods)
    {
        std::cout << g.first->name << "【" << g.first->type << "】 价格:" << g.second << "金币\n";
    }
}

void Merchant::buyItem(const std::string& goodsName, Player& player)
{
    for (auto& g : shopGoods)
    {
        if (g.first->name == goodsName)
        {
            if (player.gold >= g.second)
            {
                player.gold -= g.second;
                //复制物品
                auto newItem = std::make_shared<Item>(*g.first);
                player.pickUpItem(newItem);
            }
            else
            {
                std::cout << "金币不足！\n";
            }
            return;
        }
    }
    std::cout << "商店没有该商品\n";
}