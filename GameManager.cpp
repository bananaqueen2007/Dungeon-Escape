#include "GameManager.h"
#include <iostream>

void GameManager::showMainMenu()
{
    int select = 0;
    while (true)
    {
        std::cout << "\n========地牢逃亡========" << std::endl;
        std::cout << "1. 新建游戏" << std::endl;
        std::cout << "2. 继续游戏（读取存档）" << std::endl;
        std::cout << "请输入选择：";
        std::cin >> select;
        std::cin.ignore(); //吸收换行符

        if (select == 1)
        {
            std::string playerName;
            std::cout << "请输入冒险者名字：";
            std::getline(std::cin, playerName);
            m_player = std::make_unique<Player>(playerName);
            initRooms();
            gameLoop();
            break;
        }
        else if (select == 2)
        {
            //临时创建空玩家对象，读档覆盖数据
            m_player = std::make_unique<Player>("temp");
            bool ok = SaveIO::loadFromFile(*m_player);
            if (ok)
            {
                initRooms();
                gameLoop();
            }
            else
            {
                std::cout << "无法加载存档，请选择新建游戏！" << std::endl;
            }
            break;
        }
        else
        {
            std::cout << "输入无效，请输入1或者2！" << std::endl;
        }
    }
}

void GameManager::initRooms()
{
    //房间初始化逻辑，调用组长A实现的房间构建接口
    //此处预留，联调时直接复用A写好的8个房间数据
    std::cout << "地牢房间资源加载完成..." << std::endl;
}

void GameManager::gameLoop()
{
    std::string input;
    std::cout << "\n====冒险开始！输入指令进行操作，quit退出游戏====" << std::endl;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        CommandResult res = CommandParser::parse(input);
        handleCommand(res);
        checkGameStatus();

        //玩家死亡，跳出主循环
        if (m_player->hp <= 0)
        {
            std::cout << "游戏结束！" << std::endl;
            break;
        }
    }
}

void GameManager::handleCommand(const CommandResult& cmdRes)
{
    if (cmdRes.cmd == "inv")
    {
        m_player->showInventory();
    }
    else if (cmdRes.cmd == "skin")
    {
        m_player->showSkinList();
    }
    else if (cmdRes.cmd == "equip")
    {
        m_player->equipItem(cmdRes.arg);
    }
    else if (cmdRes.cmd == "get")
    {
        //拾取逻辑，联调对接Room地面物品
        std::cout << "执行拾取：" << cmdRes.arg << std::endl;
    }
    else if (cmdRes.cmd == "drop")
    {
        m_player->dropItem(cmdRes.arg);
    }
    else if (cmdRes.cmd == "kill")
    {
        //战斗，调用队员C战斗模块
        BattleSystem::startFight(*m_player, *m_roomList[0], cmdRes.arg);
    }
    else if (cmdRes.cmd == "talk")
    {
        ShopNpcLogic::npcTalk(*m_roomList[0], cmdRes.arg);
    }
    else if (cmdRes.cmd == "shop")
    {
        ShopNpcLogic::openShop(*m_roomList[0]);
    }
    else if (cmdRes.cmd == "buy")
    {
        ShopNpcLogic::buyGoods(*m_player, *m_roomList[0], cmdRes.arg);
    }
    else if (cmdRes.cmd == "sell")
    {
        ShopNpcLogic::sellGoods(*m_player, cmdRes.arg);
    }
    else if (cmdRes.cmd == "save")
    {
        SaveIO::saveToFile(*m_player);
    }
    else if (cmdRes.cmd == "quit")
    {
        std::cout << "执行自动存档，退出游戏。" << std::endl;
        SaveIO::saveToFile(*m_player);
        return;
    }
    else if (cmdRes.cmd == "look")
    {
        std::cout << "look：查看当前房间信息" << std::endl;
    }
    else if (cmdRes.cmd == "map")
    {
        std::cout << "map：查看地牢地图，输入数字切换房间" << std::endl;
    }
    else
    {
        std::cout << "未知指令，请重新输入！" << std::endl;
    }
}

void GameManager::checkGameStatus()
{
    //玩家死亡判定
    if (m_player->hp <= 0)
    {
        std::cout << "\n💀你的生命值归零，你倒在了地牢之中，游戏失败！" << std::endl;
    }
    //通关判定，调用C的通关检查
    if (ChestStory::checkWinCondition(*m_player))
    {
        std::cout << "🎉恭喜通关地牢逃亡！" << std::endl;
    }
}