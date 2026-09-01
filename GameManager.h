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
    //启动主菜单：新建游戏 / 读取存档
    void showMainMenu();

    //游戏主循环
    void gameLoop();

private:
    std::unique_ptr<Player> m_player;
    std::vector<std::shared_ptr<Room>> m_roomList;

    //初始化全部房间数据（调用组长A提供房间初始化接口）
    void initRooms();

    //处理解析后的每一条用户指令
    void handleCommand(const CommandResult& cmdRes);

    //边界提示：房间上锁、血量为0游戏结束等
    void checkGameStatus();
};

#endif

