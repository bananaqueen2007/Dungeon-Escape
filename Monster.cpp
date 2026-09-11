#include "Monster.h"
using namespace std;

Monster::Monster(string n, int h, int atk, int gold)
    : name(n), hp(h), attack(atk), rewardGold(gold)
{
}