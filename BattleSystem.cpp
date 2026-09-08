#include "BattleSystem.h"
#include "ChestStory.h"
#include <iostream>

extern int batKillCount;
int batKillCount = 0;

int BattleSystem::startFight(Player& player, Room& currentRoom, const std::string& monsterName)
{
    Monster* targetMonster = nullptr;
    int idx = -1;
    for (size_t i = 0; i < currentRoom.monsters.size(); i++)
    {
        auto& mon = currentRoom.monsters[i];
        if (mon->hp > 0 && mon->name == monsterName)
        {
            targetMonster = mon.get();
            idx = static_cast<int>(i);
            break;
        }
    }

    if (targetMonster == nullptr)
    {
        std::cout << "当前房间没有该活着的怪物！" << std::endl;
        return 0;
    }

    std::cout << "\n====战斗开始！对阵：" << targetMonster->name << "====" << std::endl;
    player.calcTotalAttack();

    // =========暗影斗篷选择分支=========
    bool hasCloak = (player.equipCloak != nullptr && player.equipCloak->name == "暗影斗篷");
    bool canCloakEscape = hasCloak && (targetMonster->hp < player.totalAtk);

    if (canCloakEscape)
    {
        std::cout << "\n【你穿戴暗影斗篷，可以选择隐匿逃跑！】\n";
        std::cout << "1 - 发起攻击，正常战斗\n";
        std::cout << "2 - 使用暗影斗篷隐匿逃跑（本次战斗无任何战利品，怪物依旧存活）\n";
        std::cout << "请输入选择：";
        char op;
        std::cin >> op;
        std::cin.ignore();
        if (op == '2')
        {
            std::cout << "你披上暗影斗篷悄悄隐匿逃走，没有获得任何战利品！怪物仍然在这里！\n";
            return 2; // 斗篷跳过，怪物存活
        }
    }
    // 不满足条件，则不弹出选择，直接进入战斗

    int poisonTurn = player.poisonTurn;

    while (true)
    {
        //玩家回合
        player.calcTotalAttack();
        std::cout << "【你的回合】你的攻击力：" << player.totalAtk << std::endl;
        targetMonster->hp -= player.totalAtk;
        std::cout << "你对" << targetMonster->name << "造成 " << player.totalAtk << "点伤害" << std::endl;

        //怪物死亡
        if (targetMonster->hp <= 0)
        {
            std::cout << targetMonster->name << " 被你击杀！" << std::endl;
            if (targetMonster->name == "小蝙蝠")
            {
                batKillCount++;
                if (batKillCount % 5 == 0)
                {
                    auto rustKnife = std::make_shared<Item>("生锈的刀", "攻击+3，可以卖钱", "武器", 3);
                    rustKnife->stackCount = 1;
                    std::cout << "【特殊掉落】击杀累计5只蝙蝠，掉落生锈的刀！\n";
                    player.pickUpItem(rustKnife);
                }
            }
            monsterDrop(player, currentRoom.monsters[idx]);
            return 1; //成功击杀
        }

        //怪物回合
        std::cout << "【怪物回合】" << targetMonster->name << "发起攻击！" << std::endl;
        player.takeDamage(targetMonster->attack);

        //毒蜘蛛上毒
        if (targetMonster->name == "毒蜘蛛" && poisonTurn <= 0)
        {
            poisonTurn = 3;
            player.poisonTurn = poisonTurn;
            std::cout << "你被毒蜘蛛攻击后中毒了！接下来3回合每回合扣4滴血！" << std::endl;
        }

        //毒素效果
        if (poisonTurn > 0)
        {
            poisonEffect(player, poisonTurn);
            player.poisonTurn = poisonTurn;
        }

        //回合结束清理buff
        if (player.tempBuffTurn > 0) player.tempBuffTurn--;
        if (player.reduceTurn > 0)
        {
            player.reduceTurn--;
            if (player.reduceTurn <= 0) player.damageReduce = 1.0;
        }
        if (player.tempBuffTurn <= 0) player.tempAtkBuff = 0;

        //玩家死亡判定
        if (player.hp <= 0)
        {
            std::cout << "你眼前一黑，倒在了地牢之中。游戏失败！" << std::endl;
            return 0; //玩家死亡
        }
    }
}

void BattleSystem::poisonEffect(Player& player, int& poisonTurn)
{
    player.hp -= 4;
    std::cout << "毒素侵蚀，受到4点毒素伤害！剩余毒素回合：" << poisonTurn << std::endl;
    poisonTurn--;
    if (player.hp < 0) player.hp = 0;
}

void BattleSystem::monsterDrop(Player& player, std::unique_ptr<Monster>& deadMonster)
{
    int goldGet = deadMonster->rewardGold;
    player.gold += goldGet;
    std::cout << "获得金币：" << goldGet << std::endl;

    for (auto& dropItem : deadMonster->dropItems)
    {
        std::cout << "掉落物品：" << dropItem->name << std::endl;
        if (dropItem->type == "宝石")
        {
            ChestStory::collectGem(player, dropItem->name);
        }
        else
        {
            player.pickUpItem(dropItem);
        }
    }
}