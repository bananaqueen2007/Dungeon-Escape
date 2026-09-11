#include "ChestStory.h"
#include <iostream>
#include <windows.h>

#define GREEN  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10)
#define WHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7)

using namespace std;

vector<string> ChestStory::gemList;

bool ChestStory::meetMouseKing(Player& player)
{
    cout << "\n【鼠鼠大王出现在地窖口，挡住了你的去路！】" << std::endl;
    char op;
    while (true)
    {
        cout << "选择：是否攻击鼠鼠大王？(y/n)";
        cin >> op;
        cin.ignore();
        if (op == 'y' || op == 'Y')
        {
            cout << "你竟敢攻击鼠鼠大王！你被踢回了地牢入口。" << std::endl;
            player.currentRoomId = 1;
            return false;
        }
        else if (op == 'n' || op == 'N')
        {
            if (player.hasBoneKey)
            {
                cout << "你已经拿到过鼠鼠大王的礼物了！\n";
                return true;
            }
            cout << "善良的你获得鼠鼠大王的礼物，得到骸骨密室钥匙与1500金币！" << std::endl;
            player.gold += 1500;
            player.hasBoneKey = true;
            return true;
        }
        else
        {
            cout << "输入错误！请重新输入 y/n\n";
        }
    }
}

void ChestStory::openRoomChest(Player& player, Room& curRoom)
{
    GREEN;
    if (curRoom.chestOpened)
    {
        cout << "这个房间的宝箱已经被打开过，里面空空如也。" << std::endl;
        WHITE;
        return;
    }
    cout << "你来到房间的宝箱前！" << std::endl;
    switch (curRoom.id)
    {
    case 1: //幽暗回廊：红宝石+烤鸡
        player.pickUpItem(make_shared<Item>("烤鸡", "恢复30生命", "消耗品", 0));
        player.pickUpItem(make_shared<Item>("红宝石", "最大生命值提高10，通关宝石", "宝石", 0));
        curRoom.chestOpened = true;
        break;
    case 2:
        player.pickUpItem(make_shared<Item>("吸血刀", "攻击+20，攻击造成伤害全额转为自身血量", "武器", 20));
        curRoom.chestOpened = true;
        break;
    case 3:
    {
        cout << "投入金币进行赌博(输入数字):";
        string s; getline(cin, s);
        int bet;
        try { bet = stoi(s); }
        catch (...) {
            cout << "未知输入，放弃开启宝箱。";
            WHITE;
            return;
        }
        // 修改：分开判断0负数 和 金币不足
        if (bet <= 0)
        {
            cout << "输入金额不能为0或负数";
            WHITE;
            return;
        }
        if (bet > player.gold)
        {
            cout << "你没有这么多金币！当前金币：" << player.gold;
            WHITE;
            return;
        }
        curRoom.chestOpened = true;
        if (rand() % 2 == 0)
        {
            player.gold += bet;
            cout << "翻倍！获得" << bet << "金币！";
        }
        else {
            player.gold -= bet;
            cout << "金币全部清零！";
        }
    }
    break;
    case 4:
    {
        curRoom.chestOpened = true;
        if (player.totalAtk >= 50)
        {
            player.gold += 500;
            cout << "攻击力足够，拿到500金币！";
        }
        else {
            player.hp -= 20;
            cout << "攻击力不足，损失20生命！";
        }
    }
    break;
    case 5:
    {
        curRoom.chestOpened = true;
        player.hp -= 20;
        player.gold += 1000;
        cout << "损失20生命，获得1000金币！";
    }
    break;
    case 6:
    {
        curRoom.chestOpened = true;
        cout << "碰宝箱？(y/n):"; char c; cin >> c; cin.ignore();
        if (c == 'y' || c == 'Y') { player.hp -= 10; cout << "被蜘蛛袭击，损失10血量！"; }
    }
    break;
case7:
    {
        int sel = -1;
        while (true)
        {
            cout << "1.看起来很可疑的蘑菇  2.看起来很美味的苹果，请选择1/2：";
            string selStr; getline(cin, selStr);
            try { sel = stoi(selStr); }
            catch (...) {
                cout << "\n输入错误！请输入数字1或者2\n";
                continue;
            }
            if (sel == 1 || sel == 2)
            {
                break;
            }
            else
            {
                cout << "\n输入错误！请输入数字1或者2\n";
            }
        }
        curRoom.chestOpened = true;
        if (sel == 1)
        {
            cout << "\n蘑菇剧毒！早就告诉过你很可疑了......\n";
            player.hp = 0;
            player.currentRoomId = 1;
        }
        else if (sel == 2) {
            player.maxHp += 20; player.hp += 20;
            cout << "\n苹果非常美味，最大生命+20！\n";
        }
    }
    break;
case8:
    //地牢出口无宝箱
    break;
    }
    WHITE;
    cout << endl;
}

bool ChestStory::collectGem(Player& player, const string& gemName)
{
    for (auto& g : gemList)
    {
        if (g == gemName)
        {
            cout << "你已经拥有该宝石！" << endl;
            return false;
        }
    }
    gemList.push_back(gemName);
    cout << "获得关键宝石：" << gemName << "！(" << gemList.size() << "/6)" << endl;
    return true;
}

bool ChestStory::checkWinCondition(Player& player)
{
    if (gemList.size() >= 6)
    {
        cout << "\n★你集齐全部6颗宝石！地牢大门缓缓开启！你成功逃出生天！游戏通关★" << endl;
        return true;
    }
    cout << "宝石数量不足，还不能打开地牢出口大门！当前收集：" << gemList.size() << "/6\n";
    return false;
}