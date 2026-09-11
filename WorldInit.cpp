#include "WorldInit.h"
#include <memory>
#include <vector>
#include "Merchant.h"
#include "Npc.h"   //注意：文件是Npc.h，p小写，不是NPC.h
#include "Room.h"
#include "Monster.h"
#include "Item.h"
using namespace std;

vector<std::shared_ptr<Room>> buildDungeonWorld()
{
    vector<shared_ptr<Room>> world;

    //1 幽暗回廊：Room(id,name,desc,locked,giveKey)
    auto r1 = make_shared<Room>(1, "幽暗回廊", "长长的地下主回廊，到处有蝙蝠飞舞。", false, false);
    r1->monsters.emplace_back(make_unique<Monster>("小蝙蝠", 15, 3, 10));
    world.push_back(r1);

    //2 骸骨密室【上锁】 locked=true
    auto r2 = make_shared<Room>(2, "骸骨密室", "遍地骸骨，强大骸骨守卫在此驻守。", true, false);
    r2->monsters.emplace_back(make_unique<Monster>("骸骨守卫", 100, 40, 100));
    world.push_back(r2);

    //3 贸易石室：黑市商人 + 囚徒NPC
    auto r3 = make_shared<Room>(3, "贸易石室", "地牢黑市交易地点，黑市商人在此。", false, false);
    auto merchant1 = make_shared<Merchant>("黑市商人", "欢迎来到黑市，冒险者，需要买点什么？");

    //====商店物品严格按照文档====
    // Item(名称，描述，类型，atkBonus)
    merchant1->shopGoods.emplace_back(make_shared<Item>("解毒药剂", "防御毒蜘蛛的毒", "消耗品", 0), 300);
    merchant1->shopGoods.emplace_back(make_shared<Item>("幸运药水", "最大生命上限+20", "消耗品", 0), 200);
    merchant1->shopGoods.emplace_back(make_shared<Item>("生命药水", "回满血量", "消耗品", 0), 100);
    merchant1->shopGoods.emplace_back(make_shared<Item>("鸡血", "攻击力+20，持续两个回合", "消耗品", 0), 150);
    merchant1->shopGoods.emplace_back(make_shared<Item>("防御药水", "一回合内免伤50%", "消耗品", 0), 250);

    //武器
    auto dao1 = make_shared<Item>("好刀", "攻击+10", "武器", 10);
    merchant1->shopGoods.emplace_back(dao1, 200);

    auto dao2 = make_shared<Item>("非常好的刀", "攻击+20", "武器", 20);
    merchant1->shopGoods.emplace_back(dao2, 400);

    auto dao3 = make_shared<Item>("无敌至尊宝刀", "攻击+35", "武器", 35);
    merchant1->shopGoods.emplace_back(dao3, 600);

    r3->npcs.push_back(merchant1);
    auto prisoner = make_shared<Npc>("流浪囚徒", "又进来了一个冒险者（喃喃自语），小毛头，我想我必须提醒你，最后的开门的秘密存在于六个房间里，不要因为自大遗漏任何一个……");
    r3->npcs.push_back(prisoner);
    world.push_back(r3);

    //4 积水石室
    auto r4 = make_shared<Room>(4, "积水石室", "地面积满脏水，巨型蟾蜍潜伏水中。", false, false);
    r4->monsters.emplace_back(std::make_unique<Monster>("巨型蟾蜍", 60, 20, 70));
    world.push_back(r4);

    //5 黑暗水牢
    auto r5 = make_shared<Room>(5, "黑暗水牢", "废弃水牢，腐烂尸鬼游荡。", false, false);
    r5->monsters.emplace_back(std::make_unique<Monster>("腐烂尸鬼", 70, 20, 60));
    world.push_back(r5);

    //6 蛛丝帘洞（毒蜘蛛，会施加毒素）
    auto r6 = make_shared<Room>(6, "蛛丝帘洞", "到处厚厚的蛛丝，毒蜘蛛潜藏于此。", false, false);
    r6->monsters.emplace_back(make_unique<Monster>("毒蜘蛛", 80, 24, 80));
    world.push_back(r6);

    //7 废弃地窖：第二个黑市商人 + Npc鼠鼠大王（不是怪物！）
    auto r7 = make_shared<Room>(7, "废弃地窖", "潮湿地窖，鼠鼠大王盘踞此地，也有黑市商人在此摆摊。", false, false);
    auto merchant2 = make_shared<Merchant>("黑市商人", "地窖分店，货物和上面一样，随便看看。");
    merchant2->shopGoods.assign(merchant1->shopGoods.begin(), merchant1->shopGoods.end());
    r7->npcs.push_back(merchant2);

    //鼠鼠大王是对话NPC，放入npcs，不要放到monsters
    auto mouseKing = make_shared<Npc>("鼠鼠大王", "吱吱！外来冒险者！不要攻击我，我会给予你礼物！");
    r7->npcs.push_back(mouseKing);

    world.push_back(r7);

    //8 地牢出口 BOSS深渊魔物
    auto r8 = make_shared<Room>(8, "地牢出口", "地牢的尽头，深渊魔物把守逃生大门！", false, false);
    r8->monsters.emplace_back(make_unique<Monster>("深渊魔物", 200, 50, 200));
    world.push_back(r8);

    return world;
}