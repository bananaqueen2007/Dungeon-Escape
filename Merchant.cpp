#include "Merchant.h"
#include "Player.h"
#include <iostream>

// 静态全局：全局标记幸运药水是否已经售出，所有商人共用
static bool g_soldLuckyPotion = false;

Merchant::Merchant(string name, string talk) :Npc(name, talk)
{
}

void Merchant::showShop()
{
    cout << "\n====商人商店====" << endl;
    for (auto& g : shopGoods)
    {
        //【需求7】购买幸运药水后不再显示
        if (g.first->name == "幸运药水" && g_soldLuckyPotion)
        {
            continue;
        }
        cout << g.first->name << "【" << g.first->type << "】 "
            << g.first->desc << " 价格:" << g.second << "金币\n";
    }
}

void Merchant::buyItem(const string& goodsName, Player& player)
{
    for (auto& g : shopGoods)
    {
        if (g.first->name == goodsName)
        {
            //幸运药水全局仅允许购买一次
            if (goodsName == "幸运药水" && g_soldLuckyPotion)
            {
                cout << "幸运药水已经卖光了！\n";
                return;
            }

            if (player.gold >= g.second)
            {
                player.gold -= g.second;
                auto newItem = make_shared<Item>(*g.first);
                player.pickUpItem(newItem, true);

                if (goodsName == "幸运药水")
                {
                    g_soldLuckyPotion = true;
                }
            }
            else
            {
                cout << "金币不足！\n";
            }
            return;
        }
    }
    cout << "商店没有该商品\n";
}