#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>
#include <memory>
#include "Item.h"

struct Npc
{
    std::string name;
    std::string talkText;
    Npc(std::string n, std::string t);
    virtual void talk();
    virtual ~Npc() = default;
};

#endif
