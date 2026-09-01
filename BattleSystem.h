#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H
#include <iostream>
#include <memory>
#include "Player.h"
#include "Monster.h"
#include "Room.h"

class BattleSystem
{
public:
    //·µ»Øtrue»÷É±³É¹¦£¬falseËÀÍö
    static bool startFight(Player& player, Room& currentRoom, const std::string& monsterName);
private:
    static void poisonEffect(Player& player, int& poisonTurn);
    //¹ÖÎïËÀÍö£¬µôÂä£¬±¦Ê¯ÅĞ¶Ï
    static void monsterDrop(Player& player, std::unique_ptr<Monster>& deadMonster);
};

#endif