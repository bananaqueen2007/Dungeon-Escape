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
    //临时三参数构造，兼容错误调用
    Room(int id_, std::string n_, std::string d_);
    //原来6参数构造
    Room(int id_, std::string n_, std::string d_, bool lock_, bool keyRoom, bool hasChest_ = true);
};

void initAllRooms(std::vector<std::shared_ptr<Room>>& roomList);
bool moveToRoom(int targetId, Player& player, std::vector<std::shared_ptr<Room>>& roomList);
bool roomHasAliveMonster(Room& curRoom);
bool roomHasSmallMonsterAlive(Room& curRoom, const std::string& bigMonsterName);

#endif