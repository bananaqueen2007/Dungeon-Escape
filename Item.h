#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Player;

struct Item
{
    string name;
    string desc;
    string type; //武器 /外观披风 /消耗品 /杂物 /宝石
    int atkBonus;
    int stackCount;

    Item(string n, string d, string t, int atk = 0);
    void use(Player& player);
};

#endif