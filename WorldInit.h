#ifndef WORLDINIT_H
#define WORLDINIT_H

#include <vector>
#include <memory>
#include "Room.h"
using namespace std;

vector<shared_ptr<Room>> buildDungeonWorld();

#endif