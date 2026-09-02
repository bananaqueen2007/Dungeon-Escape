#include "Npc.h"
#include <iostream>

Npc::Npc(std::string n, std::string t) :name(n), talkText(t) {}

void Npc::talk()
{
    std::cout << "【" << name << "】：" << talkText << "\n";
}