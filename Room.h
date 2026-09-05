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

    std::vector<std::unique_ptr<Monster>> monsters;
    // =========修改这里：unique_ptr → shared_ptr，适配make_shared=========
    std::vector<std::shared_ptr<Npc>> npcs;
    std::vector<std::shared_ptr<Item>> groundItems; //地面物品 get/drop使用

    Room(int id_, std::string n_, std::string d_, bool lock_, bool keyRoom);
};

//初始化全部8个房间，填充怪物、NPC、地面道具、商人商品
void initAllRooms(std::vector<std::shared_ptr<Room>>& roomList);

//map处理房间移动逻辑：增加规则，房间还有存活怪物不允许离开
bool moveToRoom(int targetId, Player& player, std::vector<std::shared_ptr<Room>>& roomList);

//检查当前房间是否还有存活怪物
bool roomHasAliveMonster(Room& curRoom);

#endif