#ifndef WORLDINIT_H
#define WORLDINIT_H
#include <vector>
#include <memory>
#include "Room.h"

std::vector<std::shared_ptr<Room>> buildDungeonWorld();

#endif