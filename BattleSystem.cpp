#include "BattleSystem.h"

bool BattleSystem::startFight(Player& player, Room& currentRoom, const std::string& monsterName)
{
    //暗影斗篷跳过战斗，无掉落
    if (player.equipCloak != nullptr && player.equipCloak->name == "暗影斗篷")
    {
        std::cout << "\n暗影斗篷生效使得你隐匿行踪，跳过本次战斗！但你因此不会获得任何战斗掉落。\n";
        return false;
    }

    Monster* targetMonster = nullptr;
    for (auto& mon : currentRoom.monsters)
    {
        if (mon->name == monsterName && !mon->isDead())
        {
            targetMonster = mon.get();
            break;
        }
    }

    if (targetMonster == nullptr)
    {
        std::cout << "当前房间不存在该怪物！\n";
        return false;
    }

    std::cout << "\n====战斗开始！对阵：" << targetMonster->name << "====\n";

    while (true)
    {
        //玩家回合攻击
        std::cout << "【你的回合】你的攻击力：" << player.totalAtk << "\n";
        targetMonster->hp -= player.totalAtk;
        std::cout << "你对" << targetMonster->name << "造成 " << player.totalAtk << "点伤害\n";

        //怪物死亡
        if (targetMonster->isDead())
        {
            std::cout << targetMonster->name << " 被你击杀！\n";
            monsterDrop(player, currentRoom.monsters[0]);
            return true;
        }

        //怪物回合攻击玩家
        std::cout << "【怪物回合】" << targetMonster->name << "发起攻击！\n";
        player.takeDamage(targetMonster->attack);

        //毒蜘蛛附加中毒状态
        if (targetMonster->name == "毒蜘蛛" && !player.poisoned)
        {
            player.poisoned = true;
            player.poisonTurns = 3;
            std::cout << "你被毒蜘蛛咬伤！中毒，接下来3回合每回合扣4滴血！\n";
        }

        //毒素效果
        if (player.poisoned)
        {
            poisonEffect(player);
        }

        //回合结束衰减鸡血buff
        player.decreaseBuffTurn();

        //玩家死亡判定
        if (player.hp <= 0)
        {
            std::cout << "💀你眼前一黑，倒在了地牢之中。游戏失败！\n";
            return false;
        }
    }
}

void BattleSystem::poisonEffect(Player& player)
{
    player.hp -= 4;
    std::cout << "毒素侵蚀，受到4点毒素伤害！剩余毒素回合：" << player.poisonTurns << "\n";
    player.poisonTurns--;
    if (player.poisonTurns <= 0)
    {
        player.poisoned = false;
        std::cout << "毒素已经消散。\n";
    }
    if (player.hp < 0) player.hp = 0;
}

void BattleSystem::monsterDrop(Player& player, std::unique_ptr<Monster>& deadMonster)
{
    int goldGet = deadMonster->rewardGold;
    player.gold += goldGet;
    std::cout << "获得金币：" << goldGet << "\n";

    for (auto& dropItem : deadMonster->dropItems)
    {
        player.pickUpItem(dropItem);
    }
}