#ifndef MERCHANT_H
#define MERCHANT_H

#include "Npc.h"
#include <vector>
#include <memory>
#include <string>
#include "Item.h"

using namespace std;

struct Merchant : public Npc
{
    Merchant(string name, string talk);
    vector<pair<shared_ptr<Item>, int>> shopGoods;
    void showShop();
    void buyItem(const string& goodsName, Player& player);
};

#endif