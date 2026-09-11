#include "GameManager.h"
#include <iostream>
#include <sstream>
#include <windows.h>
using namespace std;

//控制台颜色宏
#define RED  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12)
#define GREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10)
#define YELLOW SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14)
#define BLUE  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9)
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7)

int GameManager::safeStringToInt(const string& s)
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
    cout << "\n====可用指令列表====" << endl;
    cout << "help     : 显示指令帮助" << endl;
    cout << "map      : 查看地牢地图，输入数字切换房间" << endl;
    cout << "inv      : 打开背包面板" << endl;
    cout << "look     : 查看当前房间信息" << endl;
    cout << "use 物品名: 使用背包消耗品" << endl;
    cout << "equip 物品名 :穿戴武器" << endl;
    cout << "unequip 武器 :卸下武器放回背包" << endl;
    cout << "kill 怪物名 :攻击房间怪物" << endl;
    cout << "talk NPC名字 :和NPC对话" << endl;
    cout << "shop     : 打开商人商店" << endl;
    cout << "buy 物品名 :购买商品" << endl;
    cout << "sell 物品名 :卖出背包物品" << endl;
    cout << "chest :打开当前房间宝箱" << endl;
    cout << "save     : 手动存档" << endl;
    cout << "quit     : 保存并退出游戏\n" << endl;
    WHITE;
}

void GameManager::showMainMenu()
{
    int select = 0;
    while (true)
    {
        YELLOW;
        cout << "\n========地牢逃亡========" << endl;
        cout << "1. 新建游戏" << endl;
        cout << "2. 继续游戏（读取存档）" << endl;
        cout << "请输入选择：";
        WHITE;
        string buf;
        getline(cin, buf);
        int select = safeStringToInt(buf);

        if (select == 1)
        {
            string playerName;
            cout << "请输入冒险者名字：";
            getline(cin, playerName);
            m_player = make_unique<Player>(playerName);
            ChestStory::gemList.clear();
            initRooms();
            GREEN;
            cout << "\n你苏醒过来，发现自己身处幽暗回廊！地牢冒险正式开始！\n";
            cout << "提示：输入look查看房间完整信息\n";
            WHITE;
            gameLoop();
            break;
        }
        else if (select == 2)
        {
            m_player = make_unique<Player>("temp");
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
            cout << "输入无效，请输入1或者2！" << std::endl;
            WHITE;
        }
    }
}

void GameManager::initRooms()
{
    initAllRooms(m_roomList);
    GREEN;
    cout << "地牢房间资源加载完成..." << endl;
    WHITE;
}

void GameManager::gameLoop()
{
    string input;
    GREEN;
    cout << "\n====冒险开始！输入指令进行操作，quit退出游戏====" << endl;
    WHITE;
    printHelpText();
    while (true)
    {
        cout << "> ";
        getline(cin, input);
        CommandResult res = CommandParser::parse(input);
        handleCommand(res);

        if (m_player->hp <= 0)
        {
            RED;
            cout << "游戏结束！" << endl;
            WHITE;
            break;
        }
    }
}

void GameManager::handleCommand(const CommandResult& cmdRes)
{
    auto curRoomIt = find_if(m_roomList.begin(), m_roomList.end(), [&](auto& r) {return r->id == m_player->currentRoomId; });
    Room& curRoom = **curRoomIt;

    if (cmdRes.cmd == "inv")
    {
        YELLOW;
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
        cout << "执行自动存档，退出游戏。" << endl;
        SaveIO::saveToFile(*m_player, m_roomList);
        exit(0);
    }
    else if (cmdRes.cmd == "look")
    {
        YELLOW;
        cout << "\n【" << curRoom.name << "】" << curRoom.description << "\n";
        WHITE;
        if (curRoom.hasChest && !curRoom.chestOpened)
        {
            GREEN;
            cout << "★本房间存在未开启的宝箱！\n";
            WHITE;
        }
        cout << "----地面物品----\n";
        if (curRoom.groundItems.empty()) cout << "无\n";
        else for (auto& i : curRoom.groundItems) cout << "- " << i->name << "\n";
        cout << "----存活怪物----\n";
        bool hasMonster = false;
        for (auto& m : curRoom.monsters) { if (m->hp > 0) { cout << "- " << m->name << " hp:" << m->hp << "\n"; hasMonster = true; } }
        if (!hasMonster) std::cout << "本房间怪物已全部清除\n";
        cout << "----在场NPC----\n";
        if (curRoom.npcs.empty())
        {
            cout << "无NPC\n";
        }
        else
        {
            for (auto& n : curRoom.npcs) cout << "- " << n->name << "\n";
        }
    }
    else if (cmdRes.cmd == "map")
    {
        BLUE;
        cout << "\n====地牢房间列表====" << endl;
        for (auto& r : m_roomList)
        {
            cout << r->id << "-" << r->name;
            if (r->locked) cout << "【上锁】";
            //【需求1 删除【可获取钥匙】标签】
            cout << "\n";
        }
        WHITE;
        cout << "请输入要前往的房间数字：";
        string numStr;
        getline(cin, numStr);
        int targetId = safeStringToInt(numStr);
        if (targetId == -9999)
        {
            RED;
            cout << "未知输入，请重试\n";
            WHITE;
            return;
        }
        moveToRoom(targetId, *m_player, m_roomList);
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
        cout << "未知指令，请重新输入！" << endl;
        WHITE;
    }
}

void GameManager::checkGameStatus()
{
    if (m_player->hp <= 0)
    {
        RED;
        cout << "\n你的生命值归零，你倒在了地牢之中，游戏失败！" << endl;
        WHITE;
    }
}