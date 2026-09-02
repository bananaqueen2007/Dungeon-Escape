#ifndef MONSTER_H
#define MONSTER_H
#include <string>
#include <memory>
#include "Item.h"

struct Monster
{
    std::string name;
    int hp;
    int attack;
    int rewardGold;
    std::vector<std::shared_ptr<Item>> dropItems;

    Monster(std::string n, int h, int atk, int gold);
};

#endif
