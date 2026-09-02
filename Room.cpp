#include "Room.h"
#include "Player.h"
#include"Merchant.h"
#include "ChestStory.h"
#include <iostream>
#include <algorithm>

Room::Room(int id_, std::string n_, std::string d_, bool lock_, bool keyRoom)
    : id(id_), name(n_), description(d_), locked(lock_), giveKey(keyRoom), chestOpened(false)
{
}

bool roomHasAliveMonster(Room& curRoom)
{
    for (auto& m : curRoom.monsters)
    {
        if (m->hp > 0) return true;
    }
    return false;
}

void initAllRooms(std::vector<std::shared_ptr<Room>>& roomList)
{
    roomList.clear();
    //1‑幽暗回廊
    auto r1 = std::make_shared<Room>(1, "幽暗回廊", "长长的幽暗回廊，地牢的中心枢纽。蝙蝠在顶部盘旋。", false, false);
    //怪物：5只小蝙蝠
    for (int i = 0; i < 5; i++)
    {
        auto bat = std::make_unique<Monster>("小蝙蝠", 15, 3, 10);
        bat->dropItems.emplace_back(std::make_shared<Item>("蝙蝠的翅膀", "可以卖钱", "杂物", 0));
        bat->dropItems.emplace_back(std::make_shared<Item>("夜明砂", "卖钱", "杂物", 0));
        r1->monsters.push_back(std::move(bat));
    }
    r1->groundItems.emplace_back(std::make_shared<Item>("烤鸡", "恢复30生命", "消耗品", 0));
    roomList.push_back(r1);

    //2‑骸骨密室【上锁】
    auto r2 = std::make_shared<Room>(2, "骸骨密室", "满地骸骨，阴森的密室。巨大骸骨守卫伫立在此。", true, false);
    auto boneGuard = std::make_unique<Monster>("骸骨守卫", 100, 40, 100);
    boneGuard->dropItems.emplace_back(std::make_shared<Item>("紫宝石", "攻击+20，通关宝石", "宝石", 20));
    r2->monsters.push_back(std::move(boneGuard));
    roomList.push_back(r2);

    //3‑贸易石室【商人+流浪囚徒NPC】
    auto r3 = std::make_shared<Room>(3, "贸易石室", "商人在此处停留，可以交易物资。囚徒蜷缩墙角。", false, false);
    //流浪囚徒NPC
    auto prisoner = std::make_unique<Npc>("流浪囚徒", "又进来了一个冒险者（喃喃自语），小毛头，我想我必须提醒你，最后的开门的秘密存在于六个房间里，不要因为自大遗漏任何一个……");
    r3->npcs.push_back(std::move(prisoner));
    //黑市商人
    auto merchant = std::make_unique<Merchant>("黑市商人", "欢迎光临，冒险者！看看我的商品吧。");
    //填充商店全部商品【文档完整商品】
    merchant->shopGoods.push_back({ std::make_shared<Item>("解毒药剂","防御蜘蛛毒素","消耗品",0),300 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("幸运药水","最大生命+20","消耗品",0),200 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("生命药水","回满血量","消耗品",0),100 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("鸡血","攻击+50持续两回合","消耗品",0),150 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("防御药水","免伤50%","消耗品",0),500 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("好刀","攻击+10","武器",10),200 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("非常好的刀","攻击+20","武器",20),400 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("无敌至尊宝刀","攻击+35","武器",35),600 });
    merchant->shopGoods.push_back({ std::make_shared<Item>("暗影斗篷","可以跳过战斗，无掉落","外观披风",0),800 });
    r3->npcs.push_back(std::move(merchant));
    roomList.push_back(r3);

    //4‑积水石室
    auto r4 = std::make_shared<Room>(4, "积水石室", "地面积满冰冷地下水，蟾蜍在水中咕咕作响。", false, false);
    auto bigToad = std::make_unique<Monster>("巨型蟾蜍", 60, 20, 70);
    bigToad->dropItems.emplace_back(std::make_shared<Item>("珍贵的蟾蜍粘液", "卖高价", "杂物", 0));
    bigToad->dropItems.emplace_back(std::make_shared<Item>("黄宝石", "攻击+10，通关宝石", "宝石", 10));
    r4->monsters.push_back(std::move(bigToad));
    for (int i = 0; i < 3; i++)
    {
        auto sToad = std::make_unique<Monster>("小蟾蜍", 40, 10, 35);
        sToad->dropItems.emplace_back(std::make_shared<Item>("蟾蜍粘液", "卖钱", "杂物", 0));
        r4->monsters.push_back(std::move(sToad));
    }
    roomList.push_back(r4);

    //5‑黑暗水牢
    auto r5 = std::make_shared<Room>(5, "黑暗水牢", "曾经关押囚犯的水牢，腐烂尸鬼游荡于此。", false, false);
    auto ghoul = std::make_unique<Monster>("腐烂尸鬼", 70, 20, 60);
    ghoul->dropItems.emplace_back(std::make_shared<Item>("散发着诡异光芒的晶核", "卖钱", "杂物", 0));
    ghoul->dropItems.emplace_back(std::make_shared<Item>("蓝宝石", "获得10点免伤，通关宝石", "宝石", 0));
    r5->monsters.push_back(std::move(ghoul));
    for (int i = 0; i < 3; i++)
    {
        auto sGhoul = std::make_unique<Monster>("小尸鬼", 35, 10, 30);
        sGhoul->dropItems.emplace_back(std::make_shared<Item>("灰扑扑的晶核", "卖钱", "杂物", 0));
        r5->monsters.push_back(std::move(sGhoul));
    }
    roomList.push_back(r5);

    //6‑蛛丝帘洞
    auto r6 = std::make_shared<Room>(6, "蛛丝帘洞", "洞穴遍布厚厚的蛛网，蜘蛛沙沙爬动。", false, false);
    auto poisonSpider = std::make_unique<Monster>("毒蜘蛛", 80, 24, 80);
    poisonSpider->dropItems.emplace_back(std::make_shared<Item>("完好的蛛丝", "束缚敌人2回合", "杂物", 0));
    poisonSpider->dropItems.emplace_back(std::make_shared<Item>("绿宝石", "战后回复30血量，通关宝石", "宝石", 0));
    r6->monsters.push_back(std::move(poisonSpider));
    for (int i = 0; i < 3; i++)
    {
        auto sSpider = std::make_unique<Monster>("小蜘蛛", 40, 12, 40);
        sSpider->dropItems.emplace_back(std::make_shared<Item>("破损的蛛丝", "卖钱", "杂物", 0));
        r6->monsters.push_back(std::move(sSpider));
    }
    roomList.push_back(r6);

    //7‑废弃地窖【钥匙房间，鼠鼠大王NPC】
    auto r7 = std::make_shared<Room>(7, "废弃地窖", "废弃地窖，鼠鼠大王盘踞在此，可以拿到骸骨密室钥匙。", false, true);
    auto mouseKing = std::make_unique<Npc>("鼠鼠大王", "吱吱！外来冒险者！不要攻击我，我会给予你礼物！");
    r7->npcs.push_back(std::move(mouseKing));
    //地窖也放置商人
    auto mer2 = std::make_unique<Merchant>("黑市商人", "地窖也有我的摊位！");
    mer2->shopGoods.push_back({ std::make_shared<Item>("生命药水","回满血量","消耗品",0),100 });
    r7->npcs.push_back(std::move(mer2));
    roomList.push_back(r7);

    //8‑地牢出口【深渊魔物BOSS】
    auto r8 = std::make_shared<Room>(8, "地牢出口", "逃离地牢的大门！强大的深渊魔物守在这里。", false, false);
    auto boss = std::make_unique<Monster>("深渊魔物", 200, 50, 200);
    boss->dropItems.emplace_back(std::make_shared<Item>("黑宝石", "最终通关宝石", "宝石", 0));
    r8->monsters.push_back(std::move(boss));
    roomList.push_back(r8);
}

bool moveToRoom(int targetId, Player& player, std::vector<std::shared_ptr<Room>>& roomList)
{
    //获取当前房间
    auto curRoomIt = std::find_if(roomList.begin(), roomList.end(), [&](const std::shared_ptr<Room>& r) {return r->id == player.currentRoomId; });
    if (curRoomIt != roomList.end())
    {
        Room& cur = **curRoomIt;
        //规则：本房间还有存活怪物，禁止离开房间！文档要求kill是必要行动
        if (roomHasAliveMonster(cur))
        {
            std::cout << "\n⚠️本房间还有活着的怪物，必须击杀怪物才可以离开房间！\n";
            return false;
        }
    }

    auto it = std::find_if(roomList.begin(), roomList.end(), [&](const std::shared_ptr<Room>& r) { return r->id == targetId; });
    if (it == roomList.end())
    {
        std::cout << "没有这个房间编号！\n";
        return false;
    }
    auto& targetRoom = *it;

    if (targetRoom->locked)
    {
        if (targetRoom->id == 2 && player.hasBoneKey)
        {
            targetRoom->locked = false;
            std::cout << "✅使用钥匙打开骸骨密室大门！\n";
        }
        else
        {
            std::cout << "❌该房间已上锁，神秘的东西在门后等着你，你有种不祥的预感\n";
            return false;
        }
    }

    if (targetRoom->giveKey && !player.hasBoneKey)
    {
        //废弃地窖不会自动拿钥匙，需要和鼠鼠大王交互获取
    }

    if (targetRoom->id == 8)
    {
        if (!ChestStory::checkWinCondition(player))
        {
            return false;
        }
    }

    //幽暗回廊特殊规则：回到回廊自动刷新全部5只小蝙蝠
    if (targetRoom->id == 1)
    {
        targetRoom->monsters.clear();
        for (int i = 0; i < 5; i++)
        {
            auto bat = std::make_unique<Monster>("小蝙蝠", 15, 3, 10);
            bat->dropItems.emplace_back(std::make_shared<Item>("蝙蝠的翅膀", "可以卖钱", "杂物", 0));
            bat->dropItems.emplace_back(std::make_shared<Item>("夜明砂", "卖钱", "杂物", 0));
            targetRoom->monsters.push_back(std::move(bat));
        }
        std::cout << "⚠️回到幽暗回廊，蝙蝠重新刷新了！\n";
    }

    player.currentRoomId = targetId;
    std::cout << "🚶你来到：" << targetRoom->name << "。" << targetRoom->description << "\n";
    return true;
}