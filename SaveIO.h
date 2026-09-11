#ifndef SAVEIO_H
#define SAVEIO_H

#include <fstream>
#include <vector>
#include <memory>
#include "Player.h"
#include "Room.h"
using namespace std;

class SaveIO
{
public:
    static bool saveToFile(const Player& player, const vector<shared_ptr<Room>>& roomList);
    static bool loadFromFile(Player& player, vector<shared_ptr<Room>>& roomList);
};

#endif