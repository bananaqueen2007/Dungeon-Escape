#include "Npc.h"
#include <iostream>
using namespace std;

Npc::Npc(string n, string t)
    :name(n), talkText(t), triggeredOnce(false) {
}

void Npc::talk()
{
    cout << "¡¾" << name << "¡¿£º" << talkText << "\n";
}