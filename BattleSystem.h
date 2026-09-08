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
    //返回：1成功击杀；0玩家死亡；2暗影斗篷跳过逃跑（怪物存活）
    static int startFight(Player& player, Room& currentRoom, const std::string& monsterName);
private:
    static void poisonEffect(Player& player, int& poisonTurn);
    static void monsterDrop(Player& player, std::unique_ptr<Monster>& deadMonster);
};

#endif