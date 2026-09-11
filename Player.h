#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Item.h"
using namespace std;

class Player
{
public:
    string name;
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

    shared_ptr<Item> equipWeapon;

    vector<shared_ptr<Item>> backpack;

    const int BACKPACK_MAX_SIZE = 15;

    Player(string playerName);
    void calcTotalAttack();
    void takeDamage(int damage);
    bool useItem(const string& itemName);
    bool equipItem(const string& itemName);
    bool unequipItem(const string& type);
    bool pickUpItem(shared_ptr<Item> item, bool isBuy = false);
    shared_ptr<Item> dropItem(const string& itemName);
    void showInventory();
    bool isBackpackFull() const;
};

#endif