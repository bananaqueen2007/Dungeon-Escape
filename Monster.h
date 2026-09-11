#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <memory>
#include <vector>
#include "Item.h"
using namespace std;

struct Monster
{
    string name;
    int hp;
    int attack;
    int rewardGold;
    vector<shared_ptr<Item>> dropItems;

    Monster(string n, int h, int atk, int gold);
};

#endif