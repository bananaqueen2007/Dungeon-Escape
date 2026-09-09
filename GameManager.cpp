#include "GameManager.h"
#include <iostream>
#include <sstream>
#include <windows.h>

//控制台颜色宏
#define RED  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12)
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10)
#define YELLOW SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14)
#define BLUE  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9)
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7)

int GameManager::safeStringToInt(const std::string& s)
{
    try
    {
        return std::stoi(s);
    }
    catch (...)
    {
        return -9999;
    }
}

void GameManager::printHelpText()
{
    GREEN;
    std::cout << "\n====可用指令列表====" << std::endl;
    std::cout << "help     : 显示指令帮助" << std::endl;
    std::cout << "map      : 查看地牢地图，输入数字切换房间" << std::endl;
    std::cout << "inv      : 打开背包面板" << std::endl;
    std::cout << "look     : 查看当前房间信息" << std::endl;
    std::cout << "use 物品名: 使用背包消耗品" << std::endl;
    std::cout << "equip 物品名 :穿戴武器" << std::endl;
    std::cout << "unequip 武器 :卸下武器放回背包" << std::endl;
    std::cout << "kill 怪物名 :攻击房间怪物" << std::endl;
    std::cout << "talk NPC名字 :和NPC对话" << std::endl;
    std::cout << "shop     : 打开商人商店" << std::endl;
    std::cout << "buy 物品名 :购买商品" << std::endl;
    std::cout << "sell 物品名 :卖出背包物品" << std::endl;
    std::cout << "chest :打开当前房间宝箱" << std::endl;
    std::cout << "save     : 手动存档" << std::endl;
    std::cout << "quit     : 保存并退出游戏\n" << std::endl;
    WHITE;
}

void GameManager::showMainMenu()
{
    int select = 0;
    while (true)
    {
        YELLOW;
        std::cout << "\n========地牢逃亡========" << std::endl;
        std::cout << "1. 新建游戏" << std::endl;
        std::cout << "2. 继续游戏（读取存档）" << std::endl;
        std::cout << "请输入选择：";
        WHITE;
        std::string buf;
        std::getline(std::cin, buf);
        int select = safeStringToInt(buf);

        if (select == 1)
        {
            std::string playerName;
            std::cout << "请输入冒险者名字：";
            std::getline(std::cin, playerName);
            m_player = std::make_unique<Player>(playerName);
            ChestStory::gemList.clear();
            initRooms();
            GREEN;
            std::cout << "\n你苏醒过来，发现自己身处幽暗回廊！地牢冒险正式开始！\n";
            std::cout << "提示：输入look查看房间完整信息\n";
            WHITE;
            gameLoop();
            break;
        }
        else if (select == 2)
        {
            m_player = std::make_unique<Player>("temp");
            initRooms();
            bool ok = SaveIO::loadFromFile(*m_player, m_roomList);
            if (ok)
            {
                gameLoop();
            }
        }
        else
        {
            RED;
            std::cout << "输入无效，请输入1或者2！" << std::endl;
            WHITE;
        }
    }
}

void GameManager::initRooms()
{
    initAllRooms(m_roomList);
    GREEN;
    std::cout << "地牢房间资源加载完成..." << std::endl;
    WHITE;
}

void GameManager::gameLoop()
{
    std::string input;
    GREEN;
    std::cout << "\n====冒险开始！输入指令进行操作，quit退出游戏====" << std::endl;
    WHITE;
    printHelpText();
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        CommandResult res = CommandParser::parse(input);
        handleCommand(res);

        if (m_player->hp <= 0)
        {
            RED;
            std::cout << "游戏结束！" << std::endl;
            WHITE;
            break;
        }
    }
}

void GameManager::handleCommand(const CommandResult& cmdRes)
{
    auto curRoomIt = std::find_if(m_roomList.begin(), m_roomList.end(), [&](auto& r) {return r->id == m_player->currentRoomId; });
    Room& curRoom = **curRoomIt;

    if (cmdRes.cmd == "inv")
    {
        YELLOW; //【13】面板黄色
        m_player->showInventory();
        WHITE;
    }
    else if (cmdRes.cmd == "equip")
    {
        m_player->equipItem(cmdRes.arg);
    }
    else if (cmdRes.cmd == "unequip")
    {
        m_player->unequipItem(cmdRes.arg);
    }
    else if (cmdRes.cmd == "use")
    {
        m_player->useItem(cmdRes.arg);
    }
    else if (cmdRes.cmd == "chest")
    {
        ChestStory::openRoomChest(*m_player, curRoom);
    }
    else if (cmdRes.cmd == "kill")
    {
        int battleRet = BattleSystem::startFight(*m_player, curRoom, cmdRes.arg);
        //【12】8号房间击杀深渊魔物后校验通关
        if (battleRet == 1 && curRoom.id == 8)
        {
            bool win = ChestStory::checkWinCondition(*m_player);
            if (win)
            {
                exit(0);
            }
        }
    }
    else if (cmdRes.cmd == "talk")
    {
        ShopNpcLogic::npcTalk(curRoom, *m_player, cmdRes.arg);
    }
    else if (cmdRes.cmd == "shop")
    {
        ShopNpcLogic::openShop(curRoom);
    }
    else if (cmdRes.cmd == "buy")
    {
        ShopNpcLogic::buyGoods(*m_player, curRoom, cmdRes.arg);
    }
    else if (cmdRes.cmd == "sell")
    {
        ShopNpcLogic::sellGoods(*m_player, cmdRes.arg);
    }
    else if (cmdRes.cmd == "save")
    {
        SaveIO::saveToFile(*m_player, m_roomList);
    }
    else if (cmdRes.cmd == "quit")
    {
        std::cout << "执行自动存档，退出游戏。" << std::endl;
        SaveIO::saveToFile(*m_player, m_roomList);
        exit(0);
    }
    else if (cmdRes.cmd == "look")
    {
        YELLOW; //【13】look面板黄色
        std::cout << "\n【" << curRoom.name << "】" << curRoom.description << "\n";
        WHITE;
        if (curRoom.hasChest && !curRoom.chestOpened)
        {
            GREEN;
            std::cout << "★本房间存在未开启的宝箱！\n";
            WHITE;
        }
        std::cout << "----地面物品----\n";
        if (curRoom.groundItems.empty()) std::cout << "无\n";
        else for (auto& i : curRoom.groundItems) std::cout << "- " << i->name << "\n";
        std::cout << "----存活怪物----\n";
        bool hasMonster = false;
        for (auto& m : curRoom.monsters) { if (m->hp > 0) { std::cout << "- " << m->name << " hp:" << m->hp << "\n"; hasMonster = true; } }
        if (!hasMonster) std::cout << "本房间怪物已全部清除\n";
        std::cout << "----在场NPC----\n";
        if (curRoom.npcs.empty())
        {
            std::cout << "无NPC\n";
        }
        else
        {
            for (auto& n : curRoom.npcs) std::cout << "- " << n->name << "\n";
        }
    }
    else if (cmdRes.cmd == "map")
    {
        BLUE; //【13】地图蓝色
        std::cout << "\n====地牢房间列表====" << std::endl;
        for (auto& r : m_roomList)
        {
            std::cout << r->id << "-" << r->name;
            if (r->locked) std::cout << "【上锁】";
            if (r->giveKey) std::cout << "【可获取钥匙】";
            std::cout << "\n";
        }
        WHITE;
        std::cout << "请输入要前往的房间数字：";
        std::string numStr;
        std::getline(std::cin, numStr);
        int targetId = safeStringToInt(numStr);
        if (targetId == -9999)
        {
            RED;
            std::cout << "未知输入，请重试\n";
            WHITE;
            return;
        }
        moveToRoom(targetId, *m_player, m_roomList);
        //【12】到达8号出口房间校验通关
        if (targetId == 8)
        {
            ChestStory::checkWinCondition(*m_player);
        }
    }
    else if (cmdRes.cmd == "help")
    {
        printHelpText();
    }
    else
    {
        RED;
        std::cout << "未知指令，请重新输入！" << std::endl;
        WHITE;
    }
}

void GameManager::checkGameStatus()
{
    if (m_player->hp <= 0)
    {
        RED;
        std::cout << "\n你的生命值归零，你倒在了地牢之中，游戏失败！" << std::endl;
        WHITE;
    }
}