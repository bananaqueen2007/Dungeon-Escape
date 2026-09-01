#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <iostream>

class Player;

class Item
{
public:
    std::string name;
    std::string type;     //武器 / 消耗品 / 外观披风
    std::string desc;
    int price;
    int atkBonus;
    int hpBonus;
    int maxHpBonus;
    int atkBuffValue;     //鸡血攻击临时加成
    int buffTurn;         //buff持续回合
    int stackCount;

    Item(std::string n, std::string t, std::string d, int p, int a = 0, int h = 0, int mh = 0, int ab = 0, int bt = 0)
        : name(n), type(t), desc(d), price(p), atkBonus(a), hpBonus(h), maxHpBonus(mh), atkBuffValue(ab), buffTurn(bt), stackCount(1) {
    }

    virtual ~Item() = default;
    virtual void use(Player& player);

    void showInfo()
    {
        std::cout << "【" << name << "】[" << type << "] " << desc
            << " |攻击加成:" << atkBonus << " |价格:" << price << std::endl;
    }
};

#endif
