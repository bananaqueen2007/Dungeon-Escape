#ifndef NPC_H
#define NPC_H
#include <string>
#include <vector>
#include <memory>
#include "Item.h"

class Player;

//NPC基类
class NPC
{
public:
    std::string name;
    std::string dialogue;
    NPC(std::string n, std::string talk) : name(n), dialogue(talk) {}
    virtual ~NPC() = default;
    virtual void talk()
    {
        std::cout << "\n" << name << "说道：" << dialogue << "\n";
    }
};

//商人NPC
class Merchant : public NPC
{
public:
    std::vector<std::shared_ptr<Item>> shopGoods;
    Merchant(std::string n, std::string talk) : NPC(n, talk) {}

    void showShop()
    {
        std::cout << "\n========黑市商人商店========" << std::endl;
        for (auto& g : shopGoods)
        {
            g->showInfo();
        }
        std::cout << "===========================\n";
    }
    void buyItem(const std::string& goodsName, Player& player);
    void sellItem(const std::string& itemName, Player& player);
};

#endif
