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
    std::string name;
    int hp;
    int maxHp;
    int baseAtk;
    int totalAtk;
    int gold;
    int currentRoomId;
    bool hasBoneKey;

    //战斗临时buff
    int tempAtkBuff;
    int tempBuffTurn;
    double damageReduce;
    int reduceTurn;
    int poisonTurn;

    //修复崩溃：全部改为shared_ptr，所有权统一
    std::shared_ptr<Item> equipWeapon;
    std::shared_ptr<Item> equipCloak;

    std::vector<std::shared_ptr<Item>> backpack;
    std::vector<std::shared_ptr<Item>> skinList;

    const int BACKPACK_MAX_SIZE = 15;

    Player(std::string playerName);
    void calcTotalAttack();
    void takeDamage(int damage);
    bool useItem(const std::string& itemName);
    bool equipItem(const std::string& itemName);
    bool unequipItem(const std::string& type); //unequip 卸下装备 weapon/cloak
    bool pickUpItem(std::shared_ptr<Item> item, bool isBuy = false);
    std::shared_ptr<Item> dropItem(const std::string& itemName);
    void showInventory();
    void showSkinList();
    bool isBackpackFull() const;
};

#endif