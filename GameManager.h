#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Player.h"
#include "Room.h"
#include "CommandParser.h"
#include "SaveIO.h"
#include "BattleSystem.h"
#include "ShopNpcLogic.h"
#include "ChestStory.h"
#include <vector>

using namespace std;

class GameManager
{
public:
    void showMainMenu();
    void gameLoop();
private:
    unique_ptr<Player> m_player;
    vector<shared_ptr<Room>> m_roomList;

    void initRooms();
    void handleCommand(const CommandResult& cmdRes);
    void checkGameStatus();
    void printHelpText();
    int safeStringToInt(const string& s);
};

#endif