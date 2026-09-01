#ifndef MONSTER_H
#define MONSTER_H
#include <string>
#include <vector>
#include <memory>
#include "Item.h"

class Monster
{
public:
    std::string name;
    int hp;
    int maxHp;
    int attack;
    int rewardGold;
    std::vector<std::shared_ptr<Item>> dropItems;

    Monster(std::string n, int h, int atk, int gold)
        : name(n), hp(h), maxHp(h), attack(atk), rewardGold(gold) {
    }

    virtual ~Monster() = default;

    bool isDead()
    {
        return hp <= 0;
    }
};

#endif
