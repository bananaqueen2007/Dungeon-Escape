#include "ChestStory.h"
#include <iostream>
#include <windows.h>

#define GREEN  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10)
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7)

std::vector<std::string> ChestStory::gemList;

bool ChestStory::meetMouseKing(Player& player)
{
    std::cout << "\n【鼠鼠大王出现在地窖口，挡住了你的去路！】" << std::endl;
    std::cout << "选择：是否攻击鼠鼠大王？(y/n)";
    char op;
    std::cin >> op;
    std::cin.ignore();
    if (op == 'y' || op == 'Y')
    {
        std::cout << "你竟敢攻击鼠鼠大王！你被踢回了地牢入口。" << std::endl;
        player.currentRoomId = 1;
        return false;
    }
    else
    {
        std::cout << "善良的你获得鼠鼠大王的礼物，得到骸骨密室钥匙与1500金币！" << std::endl;
        player.gold += 1500;
        player.hasBoneKey = true;
        return true;
    }
}

void ChestStory::openRoomChest(Player& player, Room& curRoom)
{
    GREEN; //【13】宝箱绿色
    if (curRoom.chestOpened)
    {
        std::cout << "这个房间的宝箱已经被打开过，里面空空如也。" << std::endl;
        WHITE;
        return;
    }
    curRoom.chestOpened = true;
    std::cout << "你打开了房间的宝箱！" << std::endl;

    switch (curRoom.id)
    {
    case 1: //幽暗回廊：红宝石+烤鸡
        player.pickUpItem(std::make_shared<Item>("烤鸡", "恢复30生命", "消耗品", 0));
        player.pickUpItem(std::make_shared<Item>("红宝石", "最大生命值提高10，通关宝石", "宝石", 0));
        break;

    case 2:
        //【8】吸血刀改为攻击+20，100%吸血
        player.pickUpItem(std::make_shared<Item>("吸血刀", "攻击+20，攻击造成伤害全额转为自身血量", "武器", 20));
        break;

    case 3:
    {
        std::cout << "投入金币进行赌博(输入数字):";
        std::string s; std::getline(std::cin, s);
        int bet;
        try
        {
            bet = std::stoi(s);
        }
        catch (...)
        {
            std::cout << "未知输入，跳过赌博。";
            break;
        }
        //【4】赌博不能超过玩家金币
        if (bet <= 0)
        {
            std::cout << "不能输入0或者负数！";
            break;
        }
        if (bet > player.gold)
        {
            std::cout << "你没有这么多金币！当前金币：" << player.gold;
            break;
        }

        if (rand() % 2 == 0)
        {
            player.gold += bet;
            std::cout << "翻倍！获得" << bet << "金币！";
        }
        else
        {
            player.gold -= bet;
            std::cout << "金币全部清零！";
        }
    }
    break;

    case 4:
        if (player.totalAtk >= 50)
        {
            player.gold += 500;
            std::cout << "攻击力足够，拿到500金币！";
        }
        else
        {
            player.hp -= 20;
            std::cout << "攻击力不足，损失20生命！";
        }
        break;

    case 5:
        player.hp -= 20;
        player.gold += 1000;
        std::cout << "损失20生命，获得1000金币！";
        break;

    case 6:
    {
        std::cout << "碰宝箱？(y/n):";
        char c;
        std::cin >> c;
        std::cin.ignore();
        if (c == 'y' || c == 'Y')
        {
            player.hp -= 10;
            std::cout << "被蜘蛛袭击，损失10血量！";
        }
    }
    break;

    case 7:
    {
        std::cout << "1.看起来很可疑的蘑菇  2.看起来很美味的苹果，请选择1/2：";
        std::string selStr;
        std::getline(std::cin, selStr);
        int sel;
        try
        {
            sel = std::stoi(selStr);
        }
        catch (...)
        {
            std::cout << "\n未知输入！";
            break;
        }

        if (sel == 1)
        {
            std::cout << "\n蘑菇剧毒！早就告诉过你很可疑了......\n";
            player.hp = 0;
            player.currentRoomId = 1;
        }
        else if (sel == 2)
        {
            player.maxHp += 20;
            player.hp += 20;
            std::cout << "\n苹果非常美味，最大生命+20！\n";
        }
        else
        {
            std::cout << "\n未知输入！\n";
        }
    }
    break;

    case 8:
        //地牢出口无宝箱奖励
        break;
    }
    WHITE;
    std::cout << std::endl;
}

bool ChestStory::collectGem(Player& player, const std::string& gemName)
{
    for (auto& g : gemList)
    {
        if (g == gemName)
        {
            std::cout << "你已经拥有该宝石！" << std::endl;
            return false;
        }
    }
    gemList.push_back(gemName);
    std::cout << "获得关键宝石：" << gemName << "！(" << gemList.size() << "/6)" << std::endl;
    return true;
}

bool ChestStory::checkWinCondition(Player& player)
{
    if (gemList.size() >= 6)
    {
        std::cout << "\n★你集齐全部6颗宝石！地牢大门缓缓开启！你成功逃出生天！游戏通关★" << std::endl;
        return true;
    }
    std::cout << "宝石数量不足，还不能打开地牢出口大门！当前收集：" << gemList.size() << "/6\n";
    return false;
}