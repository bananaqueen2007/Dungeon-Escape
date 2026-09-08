#ifndef SAVEIO_H
#define SAVEIO_H

#include <fstream>
#include <vector>
#include <memory>
#include "Player.h"
#include "Room.h"

class SaveIO
{
public:
    static bool saveToFile(const Player& player, const std::vector<std::shared_ptr<Room>>& roomList);
    static bool loadFromFile(Player& player, std::vector<std::shared_ptr<Room>>& roomList);
};

#endif