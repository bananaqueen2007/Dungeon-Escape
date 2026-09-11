#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>
#include <memory>
#include "Item.h"
using namespace std;

struct Npc
{
    string name;
    string talkText;
    bool triggeredOnce; //是否已经触发过对话

    Npc(string n, string t);
    virtual void talk();
    virtual ~Npc() = default;
};

#endif