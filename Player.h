#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Item.h"

class Player
{
public:
    std::string name;               //角色名字
    int hp;                         //当前血量
    int maxHp;                      //最大生命上限
    int baseAtk;                    //基础攻击力
    int totalAtk;                   //总攻击力 =基础+武器+临时buff
    int gold;                       //金币
    int currentRoomId;              //当前所在房间id

    //毒素状态
    bool poisoned;
    int poisonTurns;

    //药水临时Buff：鸡血、防御药水
    int tempAtkBuff;
    int tempBuffTurn;
    double damageReduceRate;

    //穿戴装备
    std::unique_ptr<Item> equipWeapon;
    std::unique_ptr<Item> equipCloak;

    std::vector<std::shared_ptr<Item>> backpack; //背包最多15格
    std::vector<std::shared_ptr<Item>> skinList; //收集的披风外观

    const int BACKPACK_MAX_SIZE = 15;

    //构造函数
    Player(std::string playerName);

    //计算总攻击力：基础攻击 +武器加成 +临时鸡血buff
    void calcTotalAttack();

    //玩家受到伤害，自动应用减伤倍率
    void takeDamage(int rawDamage);

    //使用背包内消耗品
    bool useItem(const std::string& itemName);

    //穿戴装备/披风
    bool equipItem(const std::string& itemName);

    //拾取物品到背包，支持堆叠
    bool pickUpItem(std::shared_ptr<Item> item);

    //丢弃物品；丢弃穿戴装备自动卸下
    std::shared_ptr<Item> dropItem(const std::string& itemName);

    //inv指令：打印背包面板
    void showInventory();

    //skin指令：打印收集到的全部披风外观
    void showSkinList();

    bool isBackpackFull() const;

    //每回合结束调用，衰减鸡血buff回合
    void decreaseBuffTurn();
};

#endif
