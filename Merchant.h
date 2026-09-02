#ifndef MERCHANT_H
#define MERCHANT_H
#include "Npc.h"
#include <vector>
#include <memory>
#include "Item.h"
#include "Player.h"

struct Merchant : public Npc
{
    std::vector<std::pair<std::shared_ptr<Item>, int>> shopGoods;
    Merchant(std::string name, std::string talk);
    void showShop();
    void buyItem(const std::string& goodsName, Player& player);
};

#endif