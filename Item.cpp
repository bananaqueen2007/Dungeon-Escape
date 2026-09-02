#include "Item.h"
#include "Player.h"
#include <iostream>

Item::Item(std::string n, std::string d, std::string t, int atk)
    : name(n), desc(d), type(t), atkBonus(atk), stackCount(1)
{
}

void Item::use(Player& player)
{
    if (type == "消耗品")
    {
        if (name == "生命药水")
        {
            player.hp = player.maxHp;
            std::cout << "使用生命药水，血量回满！\n";
        }
        else if (name == "幸运药水")
        {
            player.maxHp += 20;
            player.hp += 20;
            std::cout << "使用幸运药水！最大生命+20！\n";
        }
        else if (name == "鸡血")
        {
            player.tempAtkBuff = 50;
            player.tempBuffTurn = 2;
            std::cout << "喝下鸡血！接下来2回合攻击+50！\n";
        }
        else if (name == "防御药水")
        {
            player.damageReduce = 0.5;
            player.reduceTurn = 1;
            std::cout << "防御药水生效！本回合受到伤害减半！\n";
        }
        else if (name == "解毒药剂")
        {
            player.poisonTurn = 0;
            std::cout << "解毒药剂，毒素清除！\n";
        }
        else if (name == "烤鸡")
        {
            player.hp += 30;
            if (player.hp > player.maxHp) player.hp = player.maxHp;
            std::cout << "吃下烤鸡，恢复30血量！\n";
        }
    }
}