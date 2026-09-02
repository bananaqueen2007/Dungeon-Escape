#include "Monster.h"

Monster::Monster(std::string n, int h, int atk, int gold)
    : name(n), hp(h), attack(atk), rewardGold(gold)
{
}