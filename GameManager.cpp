#include "GameManager.h"
#include <iostream>
#include <sstream>

void GameManager::printHelpText()
{
    std::cout << "\n====可用指令列表====" << std::endl;
    std::cout << "map      : 查看地牢地图，输入数字切换房间" << std::endl;
    std::cout << "inv      : 打开背包面板" << std::endl;
    std::cout << "look     : 查看当前房间信息" << std::endl;
    std::cout << "get 物品名: 拾取地面物品" << std::endl;
    std::cout << "drop 物品名:丢弃物品到地面" << std::endl;
    std::cout << "use 物品名: 使用背包消耗品" << std::endl;
    std::cout << "equip 物品名 :穿戴武器/披风" << std::endl;
    std::cout << "kill 怪物名 :攻击房间怪物" << std::endl;
    std::cout << "talk NPC名字 :和NPC对话" << std::endl;
    std::cout << "shop     : 打开商人商店" << std::endl;
    std::cout << "buy 物品名 :购买商品" << std::endl;
    std::cout << "sell 物品名 :卖出背包物品" << std::endl;
    std::cout << "chest :打开当前房间宝箱" << std::endl;
    std::cout << "skin     : 查看已收集披风外观" << std::endl;
    std::cout << "save     : 手动存档" << std::endl;
    std::cout << "quit     : 保存并退出游戏" << std::endl;
    std::cout << "help     : 再次显示指令帮助\n" << std::endl;
}

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
        std::cin.ignore();

        if (select == 1)
        {
            std::string playerName;
            std::cout << "请输入冒险者名字：";
            std::getline(std::cin, playerName);
            m_player = std::make_unique<Player>(playerName);
            ChestStory::gemList.clear();
            initRooms();
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
            std::cout << "输入无效，请输入1或者2！" << std::endl;
        }
    }
}

void GameManager::initRooms()
{
    initAllRooms(m_roomList);
    std::cout << "地牢房间资源加载完成..." << std::endl;
}

void GameManager::gameLoop()
{
    std::string input;
    std::cout << "\n====冒险开始！输入指令进行操作，quit退出游戏====" << std::endl;
    printHelpText();
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);
        CommandResult res = CommandParser::parse(input);
        handleCommand(res);
        checkGameStatus();

        if (m_player->hp <= 0)
        {
            std::cout << "游戏结束！" << std::endl;
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
    else if (cmdRes.cmd == "use")
    {
        m_player->useItem(cmdRes.arg);
    }
    else if (cmdRes.cmd == "get")
    {
        bool found = false;
        for (size_t i = 0; i < curRoom.groundItems.size(); i++)
        {
            if (curRoom.groundItems[i]->name == cmdRes.arg)
            {
                auto it = curRoom.groundItems[i];
                if (m_player->pickUpItem(it))
                {
                    curRoom.groundItems.erase(curRoom.groundItems.begin() + i);
                }
                found = true;
                break;
            }
        }
        if (!found) std::cout << "地面没有这个物品！\n";
    }
    else if (cmdRes.cmd == "drop")
    {
        auto item = m_player->dropItem(cmdRes.arg);
        if (item != nullptr) curRoom.groundItems.push_back(item);
    }
    else if (cmdRes.cmd == "chest")
    {
        ChestStory::openRoomChest(*m_player, curRoom);
    }
    else if (cmdRes.cmd == "kill")
    {
        BattleSystem::startFight(*m_player, curRoom, cmdRes.arg);
    }
    else if (cmdRes.cmd == "talk")
    {
        ShopNpcLogic::npcTalk(curRoom, *m_player,cmdRes.arg);
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
        return;
    }
    else if (cmdRes.cmd == "look")
    {
        std::cout << "\n【" << curRoom.name << "】" << curRoom.description << "\n";
        std::cout << "----地面物品----\n";
        if (curRoom.groundItems.empty()) std::cout << "无\n";
        else for (auto& i : curRoom.groundItems) std::cout << "- " << i->name << "\n";
        std::cout << "----存活怪物----\n";
        bool hasMonster = false;
        for (auto& m : curRoom.monsters) { if (m->hp > 0) { std::cout << "- " << m->name << " hp:" << m->hp << "\n"; hasMonster = true; } }
        if (!hasMonster) std::cout << "本房间怪物已全部清除\n";
        std::cout << "----在场NPC----\n";
        for (auto& n : curRoom.npcs) std::cout << "- " << n->name << "\n";
    }
    else if (cmdRes.cmd == "map")
    {
        std::cout << "\n====地牢房间列表====" << std::endl;
        for (auto& r : m_roomList)
        {
            std::cout << r->id << "-" << r->name;
            if (r->locked) std::cout << "【上锁】";
            if (r->giveKey) std::cout << "【可获取钥匙】";
            std::cout << "\n";
        }
        std::cout << "请输入要前往的房间数字：";
        std::string numStr;
        std::getline(std::cin, numStr);
        int targetId = std::stoi(numStr);
        moveToRoom(targetId, *m_player, m_roomList);
    }
    else if (cmdRes.cmd == "help")
    {
        printHelpText();
    }
    else
    {
        std::cout << "未知指令，请重新输入！" << std::endl;
    }
}

void GameManager::checkGameStatus()
{
    if (m_player->hp <= 0)
    {
        std::cout << "\n💀你的生命值归零，你倒在了地牢之中，游戏失败！" << std::endl;
    }
    ChestStory::checkWinCondition(*m_player);
}