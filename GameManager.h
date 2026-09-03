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

class GameManager
{
public:
    void showMainMenu();
    void gameLoop();
private:
    std::unique_ptr<Player> m_player;
    std::vector<std::shared_ptr<Room>> m_roomList;

    void initRooms();
    void handleCommand(const CommandResult& cmdRes);
    void checkGameStatus();
    void printHelpText();
};

#endif