#include "ChestStory.h"
#include <iostream>

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
    if (curRoom.chestOpened)
    {
        std::cout << "这个房间的宝箱已经被打开过，里面空空如也。" << std::endl;
        return;
    }
    curRoom.chestOpened = true;
    std::cout << "你打开了房间的宝箱！" << std::endl;
    //各个房间宝箱逻辑
    switch (curRoom.id)
    {
    case 1: //幽暗回廊
        player.pickUpItem(std::make_shared<Item>("烤鸡", "恢复30生命", "消耗品", 0));
        break;
    case 2: //骸骨密室
        player.pickUpItem(std::make_shared<Item>("吸血刀", "攻击+10攻击回血", "武器", 10));
        break;
    case 3: //贸易石室赌博宝箱
    {
        std::cout << "投入金币进行赌博(输入数字):";
        int bet; std::cin >> bet; std::cin.ignore();
        if (rand() % 2 == 0)
        {
            player.gold += bet;
            std::cout << "🎉翻倍！获得" << bet << "金币！";
        }
        else {
            player.gold -= bet;
            std::cout << "💸金币全部清零！";
        }
    }
    break;
    case 4: //积水石室
        if (player.totalAtk >= 50)
        {
            player.gold += 500;
            std::cout << "✅攻击力足够，拿到500金币！";
        }
        else {
            player.hp -= 20;
            std::cout << "❌攻击力不足，损失20生命！";
        }
        break;
    case 5://黑暗水牢吸血宝箱
        player.hp -= 20;
        player.gold += 1000;
        std::cout << "损失20生命，获得1000金币！";
        break;
    case 6://蛛丝帘洞
    {
        std::cout << "碰宝箱？(y/n):"; char c; std::cin >> c; std::cin.ignore();
        if (c == 'y' || c == 'Y') { player.hp -= 10; std::cout << "被蜘蛛袭击，损失10血量！"; }
    }
    break;
case7://废弃地窖二选一
    {
        std::cout << "1.看起来很可疑的蘑菇  2.看起来很美味的苹果，请选择1/2：";
        int sel; std::cin >> sel; std::cin.ignore();
        if (sel == 1)
        {
            player.hp = 0;
            player.currentRoomId = 1;
            std::cout << "蘑菇剧毒！早就告诉过你很可疑了......";
        }
        else {
            player.maxHp += 20; player.hp += 20;
            std::cout << "苹果非常美味，最大生命+20！";
        }
    }
    break;
case8:
    break;
    }
    std::cout << std::endl;
}

bool ChestStory::canSkipBattle(Player& player)
{
    if (player.equipCloak != nullptr && player.equipCloak->name == "暗影斗篷")
    {
        std::cout << "暗影斗篷生效，你隐匿行踪，跳过本次战斗！不会获得任何战斗掉落！\n";
        return true;
    }
    return false;
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
    std::cout << "获得关键宝石：" << gemName << "！(" << gemList.size() << "/8)" << std::endl;
    return true;
}

bool ChestStory::checkWinCondition(Player& player)
{
    if (gemList.size() >= 8)
    {
        std::cout << "\n★你集齐全部8颗宝石！地牢大门缓缓开启！你成功逃出生天！游戏通关★" << std::endl;
        return true;
    }
    std::cout << "宝石数量不足，还不能打开地牢出口大门！当前收集：" << gemList.size() << "/8\n";
    return false;
}