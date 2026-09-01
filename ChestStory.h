#ifndef CHESTSTORY_H
#define CHESTSTORY_H
#include "Player.h"
#include "Room.h"
#include <vector>

class ChestStory
{
public:
    static bool meetMouseKing(Player& player);
    static void openRoomChest(Player& player, Room& curRoom);
    static bool canSkipBattle(Player& player);
    static bool collectGem(Player& player, const std::string& gemName);
    static bool checkWinCondition(Player& player);
    static std::vector<std::string> gemList;
};

#endif