#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <vector>
#include <memory>
#include "Monster.h"
#include "NPC.h"

class Room
{
public:
    int id;
    std::string name;
    std::string description;
    bool isLocked;
    bool chestOpened;

    std::vector<std::shared_ptr<Item>> groundItems;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::shared_ptr<NPC>> npcs;

    Room(int id_, std::string name_, std::string desc_)
        : id(id_), name(name_), description(desc_), isLocked(false), chestOpened(false) {
    }

    void showRoomInfo()
    {
        std::cout << "\n====【" << name << "】 ID:" << id << " ====\n";
        std::cout << description << "\n";
        if (isLocked)
        {
            std::cout << "⚠该房间房门处于上锁状态。\n";
        }
        std::cout << "====================================\n";
    }
};

#endif
