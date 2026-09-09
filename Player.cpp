#include "Player.h"
#include"ChestStory.h"

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
    backpack.clear();
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

bool Player::pickUpItem(std::shared_ptr<Item> item, bool isBuy)
{
    if (isBackpackFull())
    {
        std::cout << "背包已满，无法拾取物品！" << std::endl;
        return false;
    }

    //【宝石自动拾取生效】
    if (item->type == "宝石")
    {
        ChestStory::collectGem(*this, item->name);
        //红宝石特殊：maxHp+10
        if (item->name == "红宝石")
        {
            maxHp += 10;
            hp += 10;
            std::cout << "红宝石力量涌动！最大生命值+10\n";
        }
        return true;
    }

    for (auto& p : backpack)
    {
        if (p->name == item->name)
        {
            p->stackCount += item->stackCount;
            if (!isBuy)
                std::cout << "拾取到了：" << item->name << std::endl;
            else
                std::cout << "购买成功：" << item->name << std::endl;
            return true;
        }
    }
    backpack.push_back(item);
    if (!isBuy)
        std::cout << "拾取到了：" << item->name << std::endl;
    else
        std::cout << "购买成功：" << item->name << std::endl;
    return true;
}

std::shared_ptr<Item> Player::dropItem(const std::string& itemName)
{
    for (size_t i = 0; i < backpack.size(); i++)
    {
        if (backpack[i]->name == itemName)
        {
            //【18】红宝石禁止卖出
            if (backpack[i]->name == "红宝石")
            {
                std::cout << "红宝石是关键道具，不能丢弃/卖出！\n";
                return nullptr;
            }

            auto res = backpack[i];
            if (equipWeapon != nullptr && equipWeapon->name == itemName)
            {
                equipWeapon = nullptr;
                calcTotalAttack();
                std::cout << "卸下已装备武器！" << std::endl;
            }
            backpack.erase(backpack.begin() + i);
            std::cout << "丢弃物品：" << res->name << std::endl;
            return res;
        }
    }
    if (equipWeapon && equipWeapon->name == itemName)
    {
        auto res = equipWeapon;
        equipWeapon = nullptr;
        calcTotalAttack();
        std::cout << "卸下已装备武器，丢弃！" << std::endl;
        return res;
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
                if (equipWeapon != nullptr)
                {
                    backpack.push_back(equipWeapon);
                    std::cout << "旧武器放回背包\n";
                }
                equipWeapon = it;
                for (auto iter = backpack.begin(); iter != backpack.end(); ++iter)
                {
                    if (*iter == it)
                    {
                        backpack.erase(iter);
                        break;
                    }
                }
                calcTotalAttack();
                std::cout << "装备武器成功！总攻击力：" << totalAtk << std::endl;
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

bool Player::unequipItem(const std::string& type)
{
    if (type == "weapon" || type == "武器")
    {
        if (!equipWeapon)
        {
            std::cout << "当前没有装备武器\n";
            return false;
        }
        if (isBackpackFull())
        {
            std::cout << "背包已满，无法卸下！\n";
            return false;
        }
        backpack.push_back(equipWeapon);
        std::cout << "卸下武器：" << equipWeapon->name << "放回背包\n";
        equipWeapon = nullptr;
        calcTotalAttack();
        return true;
    }
    else
    {
        std::cout << "unequip 参数：武器\n";
        return false;
    }
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
    std::cout << "宝石收集数量:" << ChestStory::gemList.size() << "/6 ";
    if (hasBoneKey) std::cout << "【持有骸骨密室钥匙】";
    std::cout << std::endl;

    if (equipWeapon)
        std::cout << "已装备武器:" << equipWeapon->name << std::endl;
    else
        std::cout << "已装备武器:无" << std::endl;

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

bool Player::isBackpackFull() const
{
    return backpack.size() >= BACKPACK_MAX_SIZE;
}
