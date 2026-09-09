#ifndef MERCHANT_H
#define MERCHANT_H

#include "Npc.h"
#include <vector>
#include <memory>
#include <string>
#include "Item.h"

struct Merchant : public Npc
{
    Merchant(std::string name, std::string talk);
    std::vector<std::pair<std::shared_ptr<Item>, int>> shopGoods;
    void showShop();
    void buyItem(const std::string& goodsName, Player& player);
};

#endif