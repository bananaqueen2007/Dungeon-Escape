#ifndef ROOM_H
#define ROOM_H


#include <string>
#include <vector>
#include <memory>
#include "Monster.h"
#include "Npc.h"
#include "Item.h"
#include "Player.h"


struct Room
{
    int id;
    std::string name;
    std::string description;
    bool locked;
    bool giveKey;
    bool chestOpened;
    bool hasChest;


    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::shared_ptr<Npc>> npcs;
    std::vector<std::shared_ptr<Item>> groundItems;


    Room() = default;
    Room(int id_, std::string n_, std::string d_);
    Room(int id_, std::string n_, std::string d_, bool lock_, bool keyRoom, bool hasChest_ = true);
};


void initAllRooms(std::vector<std::shared_ptr<Room>>& roomList);
bool moveToRoom(int targetId, Player& player, std::vector<std::shared_ptr<Room>>& roomList);
bool roomHasAliveMonster(Room& curRoom);

#endif