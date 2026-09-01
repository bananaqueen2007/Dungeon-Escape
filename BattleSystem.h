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
    // 返回true击杀成功；false=玩家死亡 / 斗篷跳过战斗
    static bool startFight(Player& player, Room& currentRoom, const std::string& monsterName);

private:
    //毒素每回合扣血
    static void poisonEffect(Player& player);
    //怪物死亡处理金币、物品掉落
    static void monsterDrop(Player& player, std::unique_ptr<Monster>& deadMonster);
};

#endif