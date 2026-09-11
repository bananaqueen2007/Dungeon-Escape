#ifndef ROOM_H
#define ROOM_H


#include <string>
#include <vector>
#include <memory>
#include "Monster.h"
#include "Npc.h"
#include "Item.h"
#include "Player.h"
using namespace std;


struct Room
{
    int id;
    string name;
    string description;
    bool locked;
    bool giveKey;
    bool chestOpened;
    bool hasChest;


    vector<unique_ptr<Monster>> monsters;
    vector<shared_ptr<Npc>> npcs;
    vector<shared_ptr<Item>> groundItems;


    Room() = default;
    Room(int id_, string n_, string d_);
    Room(int id_, string n_, string d_, bool lock_, bool keyRoom, bool hasChest_ = true);
};


void initAllRooms(vector<shared_ptr<Room>>& roomList);
bool moveToRoom(int targetId, Player& player, vector<shared_ptr<Room>>& roomList);
bool roomHasAliveMonster(Room& curRoom);

#endif