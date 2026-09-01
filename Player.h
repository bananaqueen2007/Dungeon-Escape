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

    //’Ω∂∑¡Ÿ ±buff
    int tempAtkBuff;
    int tempBuffTurn;
    double damageReduce;
    int reduceTurn;
    int poisonTurn;

    std::unique_ptr<Item> equipWeapon;
    std::unique_ptr<Item> equipCloak;

    std::vector<std::shared_ptr<Item>> backpack;
    std::vector<std::shared_ptr<Item>> skinList;

    const int BACKPACK_MAX_SIZE = 15;

    Player(std::string playerName);
    void calcTotalAttack();
    void takeDamage(int damage);
    bool useItem(const std::string& itemName);
    bool equipItem(const std::string& itemName);
    bool pickUpItem(std::shared_ptr<Item> item);
    std::shared_ptr<Item> dropItem(const std::string& itemName);
    void showInventory();
    void showSkinList();
    bool isBackpackFull() const;
};

#endif
