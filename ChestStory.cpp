#include "ChestStory.h"
#include <iostream>

std::vector<std::string> ChestStory::gemList;

bool ChestStory::meetMouseKing(Player& player)
{
    std::cout << "\n【鼠鼠大王出现在地窖口，挡住了你的去路！】\n";
    std::cout << "选择：是否攻击鼠鼠大王？(y/n)：";
    char op;
    std::cin >> op;
    std::cin.ignore();

    if (op == 'y' || op == 'Y')
    {
        std::cout << "竟敢攻击鼠鼠大王！你被一脚踢回地牢入口。\n";
        player.currentRoomId = 1;
        return false;
    }
    else
    {
        std::cout << "善良的你获得鼠鼠大王的礼物，得到骸骨密室钥匙与1500金币！\n";
        player.gold += 1500;
        return true;
    }
}

void ChestStory::openRoomChest(Player& player, Room& curRoom)
{
    if (curRoom.chestOpened)
    {
        std::cout << "该房间宝箱已经打开过，里面空空如也。\n";
        return;
    }
    curRoom.chestOpened = true;
    std::cout << "你打开了房间的宝箱！\n";
}

bool ChestStory::collectGem(const std::string& gemName)
{
    for (auto& g : gemList)
    {
        if (g == gemName)
        {
            std::cout << "你已经拥有该宝石！\n";
            return false;
        }
    }
    gemList.push_back(gemName);
    std::cout << "获得关键宝石：" << gemName << "！(" << gemList.size() << "/8)\n";
    return true;
}

bool ChestStory::checkWinCondition()
{
    if (gemList.size() >= 8)
    {
        std::cout << "\n★你集齐全部8颗宝石！成功开启了地牢大门！游戏通关★\n";
        return true;
    }
    std::cout << "宝石数量不足，还不能打开地牢大门！当前收集：" << gemList.size() << "/8\n";
    return false;
}