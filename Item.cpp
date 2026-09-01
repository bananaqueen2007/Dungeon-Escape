#include "Item.h"
#include "Player.h"

void Item::use(Player& player)
{
    if (name == "解毒药剂")
    {
        player.poisoned = false;
        player.poisonTurns = 0;
        std::cout << "使用解毒药剂，身上毒素已经清除！\n";
        return;
    }
    if (name == "幸运药水")
    {
        player.maxHp += 20;
        player.hp += 20;
        if (player.hp > player.maxHp) player.hp = player.maxHp;
        std::cout << "使用幸运药水！最大生命上限+20\n";
        return;
    }
    if (name == "生命药水")
    {
        player.hp = player.maxHp;
        std::cout << "使用生命药水，血量完全回满！\n";
        return;
    }
    if (name == "鸡血")
    {
        player.tempAtkBuff = 50;
        player.tempBuffTurn = 2;
        std::cout << "喝下鸡血！攻击力+50，持续2回合！\n";
        return;
    }
    if (name == "防御药水")
    {
        player.damageReduceRate = 0.5;
        std::cout << "喝下防御药水，受到伤害减免50%！本道具仅可购买一次\n";
        return;
    }
    if (hpBonus > 0)
    {
        player.hp += hpBonus;
        if (player.hp > player.maxHp) player.hp = player.maxHp;
        std::cout << "使用" << name << "，恢复" << hpBonus << "生命值！\n";
    }
}