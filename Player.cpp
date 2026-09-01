#include "Player.h"

Player::Player(std::string playerName)
{
    name = playerName;
    hp = 50;
    maxHp = 100;
    baseAtk = 20;
    totalAtk = 20;
    gold = 200;
    currentRoomId = 1;
    hasBoneKey = false;

    tempAtkBuff = 0;
    tempBuffTurn = 0;
    damageReduce = 1.0;
    reduceTurn = 0;
    poisonTurn = 0;

    equipWeapon = nullptr;
    equipCloak = nullptr;
    backpack.clear();
    skinList.clear();
}

void Player::calcTotalAttack()
{
    totalAtk = baseAtk + tempAtkBuff;
    if (equipWeapon != nullptr)
    {
        totalAtk += equipWeapon->atkBonus;
    }
}

void Player::takeDamage(int damage)
{
    int realDmg = static_cast<int>(damage * damageReduce);
    hp -= realDmg;
    if (hp < 0) hp = 0;
    std::cout << name << " 受到 " << realDmg << " 点伤害！剩余血量：" << hp << std::endl;
}

bool Player::pickUpItem(std::shared_ptr<Item> item)
{
    if (isBackpackFull())
    {
        std::cout << "背包已满，无法拾取物品！" << std::endl;
        return false;
    }
    for (auto& p : backpack)
    {
        if (p->name == item->name)
        {
            p->stackCount += item->stackCount;
            return true;
        }
    }
    backpack.push_back(item);
    std::cout << "拾取成功：" << item->name << std::endl;
    return true;
}

std::shared_ptr<Item> Player::dropItem(const std::string& itemName)
{
    for (size_t i = 0; i < backpack.size(); i++)
    {
        if (backpack[i]->name == itemName)
        {
            auto res = backpack[i];
            if (equipWeapon != nullptr && equipWeapon->name == itemName)
            {
                equipWeapon.reset();
                calcTotalAttack();
                std::cout << "卸下已装备武器！" << std::endl;
            }
            if (equipCloak != nullptr && equipCloak->name == itemName)
            {
                equipCloak.reset();
                std::cout << "卸下已装备披风！" << std::endl;
            }
            backpack.erase(backpack.begin() + i);
            std::cout << "丢弃物品：" << res->name << std::endl;
            return res;
        }
    }
    std::cout << "背包找不到该物品！" << std::endl;
    return nullptr;
}

bool Player::equipItem(const std::string& itemName)
{
    for (auto& it : backpack)
    {
        if (it->name == itemName)
        {
            if (it->type == "武器")
            {
                equipWeapon.reset(it.get());
                calcTotalAttack();
                std::cout << "装备武器成功！总攻击力：" << totalAtk << std::endl;
                return true;
            }
            else if (it->type == "外观披风")
            {
                equipCloak.reset(it.get());
                skinList.push_back(it);
                std::cout << "穿戴披风成功！" << std::endl;
                return true;
            }
            else
            {
                std::cout << "该物品不能穿戴！" << std::endl;
                return false;
            }
        }
    }
    std::cout << "背包没有这个物品！" << std::endl;
    return false;
}

bool Player::useItem(const std::string& itemName)
{
    for (size_t i = 0; i < backpack.size(); ++i)
    {
        auto& it = backpack[i];
        if (it->name == itemName && it->type == "消耗品")
        {
            it->use(*this);
            it->stackCount--;
            if (it->stackCount <= 0)
            {
                backpack.erase(backpack.begin() + i);
            }
            calcTotalAttack();
            return true;
        }
    }
    std::cout << "未找到可使用的消耗品" << std::endl;
    return false;
}

void Player::showInventory()
{
    std::cout << "\n===== 角色背包面板 =====" << std::endl;
    std::cout << "姓名:" << name << " 血量:" << hp << "/" << maxHp << std::endl;
    std::cout << "基础攻击:" << baseAtk << "总攻击:" << totalAtk << "金币:" << gold << std::endl;
    std::cout << "宝石收集数量:" << ChestStory::gemList.size() << "/8 ";
    if (hasBoneKey) std::cout << "【持有骸骨密室钥匙】";
    std::cout << std::endl;

    if (equipWeapon)
        std::cout << "已装备武器:" << equipWeapon->name << std::endl;
    else
        std::cout << "已装备武器:无" << std::endl;

    if (equipCloak)
        std::cout << "已装备披风:" << equipCloak->name << std::endl;
    else
        std::cout << "已装备披风:无" << std::endl;

    std::cout << "-----背包物品-----" << std::endl;
    if (backpack.empty())
    {
        std::cout << "(背包为空)" << std::endl;
    }
    else
    {
        for (auto& item : backpack)
        {
            std::cout << "[" << item->name << "] x" << item->stackCount << " | " << item->desc << "【" << item->type << "】" << std::endl;
        }
    }
    std::cout << "======================\n" << std::endl;
}

void Player::showSkinList()
{
    std::cout << "\n====已收集披风外观skin====" << std::endl;
    if (skinList.empty())
    {
        std::cout << "暂未收集任何披风" << std::endl;
    }
    else
    {
        for (auto& s : skinList)
        {
            std::cout << "- " << s->name << "  " << s->desc << std::endl;
        }
    }
    std::cout << "==========================\n";
}

bool Player::isBackpackFull() const
{
    return backpack.size() >= BACKPACK_MAX_SIZE;
}
