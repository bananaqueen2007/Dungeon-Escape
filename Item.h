#ifndef ITEM_H
#define ITEM_H
#include <string>

class Player;

struct Item
{
    std::string name;
    std::string desc;
    std::string type; //武器 /外观披风 /消耗品 /杂物 /宝石
    int atkBonus;
    int stackCount;

    Item(std::string n, std::string d, std::string t, int atk = 0);
    void use(Player& player);
};

#endif
