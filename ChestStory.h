#ifndef CHESTSTORY_H
#define CHESTSTORY_H

#include "Player.h"
#include "Room.h"
#include <vector>

using namespace std;

class ChestStory
{
public:
    static bool meetMouseKing(Player& player);
    static void openRoomChest(Player& player, Room& curRoom);
    static bool collectGem(Player& player, const string& gemName);
    static bool checkWinCondition(Player& player);
    static vector<string> gemList;
};

#endif