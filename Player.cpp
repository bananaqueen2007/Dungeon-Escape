#include "Player.h"
#include"ChestStory.h"
using namespace std;

Player::Player(string playerName)
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
    cout << name << " 受到 " << realDmg << " 点伤害！剩余血量：" << hp << endl;
}

bool Player::pickUpItem(shared_ptr<Item> item, bool isBuy)
{
    if (isBackpackFull())
    {
        cout << "背包已满，无法拾取物品！" << endl;
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
            cout << "红宝石力量涌动！最大生命值+10\n";
        }
        return true;
    }

    for (auto& p : backpack)
    {
        if (p->name == item->name)
        {
            p->stackCount += item->stackCount;
            if (!isBuy)
                cout << "拾取到了：" << item->name << endl;
            else
                cout << "购买成功：" << item->name << endl;
            return true;
        }
    }
    backpack.push_back(item);
    if (!isBuy)
        cout << "拾取到了：" << item->name << endl;
    else
        cout << "购买成功：" << item->name << endl;
    return true;
}

shared_ptr<Item> Player::dropItem(const string& itemName)
{
    for (size_t i = 0; i < backpack.size(); i++)
    {
        if (backpack[i]->name == itemName)
        {
            //红宝石禁止卖出
            if (backpack[i]->name == "红宝石")
            {
                cout << "红宝石是关键道具，不能丢弃/卖出！\n";
                return nullptr;
            }

            auto res = backpack[i];
            if (equipWeapon != nullptr && equipWeapon->name == itemName)
            {
                equipWeapon = nullptr;
                calcTotalAttack();
                cout << "卸下已装备武器！" << endl;
            }
            backpack.erase(backpack.begin() + i);
            cout << "丢弃物品：" << res->name << endl;
            return res;
        }
    }
    if (equipWeapon && equipWeapon->name == itemName)
    {
        auto res = equipWeapon;
        equipWeapon = nullptr;
        calcTotalAttack();
        cout << "卸下已装备武器，丢弃！" << endl;
        return res;
    }

    cout << "背包找不到该物品！" << endl;
    return nullptr;
}

bool Player::equipItem(const string& itemName)
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
                    cout << "旧武器放回背包\n";
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
                cout << "装备武器成功！总攻击力：" << totalAtk << endl;
                return true;
            }
            else
            {
                cout << "该物品不能穿戴！" << endl;
                return false;
            }
        }
    }
    cout << "背包没有这个物品！" << endl;
    return false;
}

bool Player::unequipItem(const string& type)
{
    if (type == "weapon" || type == "武器")
    {
        if (!equipWeapon)
        {
            cout << "当前没有装备武器\n";
            return false;
        }
        if (isBackpackFull())
        {
            cout << "背包已满，无法卸下！\n";
            return false;
        }
        backpack.push_back(equipWeapon);
        cout << "卸下武器：" << equipWeapon->name << "放回背包\n";
        equipWeapon = nullptr;
        calcTotalAttack();
        return true;
    }
    else
    {
        cout << "unequip 参数：武器\n";
        return false;
    }
}

bool Player::useItem(const string& itemName)
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
    cout << "未找到可使用的消耗品" << endl;
    return false;
}

void Player::showInventory()
{
    cout << "\n===== 角色背包面板 =====" << endl;
    cout << "姓名:" << name << " 血量:" << hp << "/" << maxHp << endl;
    cout << "基础攻击:" << baseAtk << "总攻击:" << totalAtk << "金币:" << gold << endl;
    cout << "宝石收集数量:" << ChestStory::gemList.size() << "/6 ";
    if (hasBoneKey) cout << "【持有骸骨密室钥匙】";
    cout << endl;

    if (equipWeapon)
        cout << "已装备武器:" << equipWeapon->name << endl;
    else
        cout << "已装备武器:无" << endl;

    cout << "-----背包物品-----" << endl;
    if (backpack.empty())
    {
        cout << "(背包为空)" << endl;
    }
    else
    {
        for (auto& item : backpack)
        {
            cout << "[" << item->name << "] x" << item->stackCount << " | " << item->desc << "【" << item->type << "】" << endl;
        }
    }
    cout << "======================\n" << endl;
}

bool Player::isBackpackFull() const
{
    return backpack.size() >= BACKPACK_MAX_SIZE;
}
