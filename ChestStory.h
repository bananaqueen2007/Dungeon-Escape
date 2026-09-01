#ifndef CHESTSTORY_H
#define CHESTSTORY_H

#include "Player.h"
#include "Room.h"
#include <vector>
#include <string>

class ChestStory
{
public:
    //鼠鼠大王交互，true拿到奖励；false被踢回入口
    static bool meetMouseKing(Player& player);
    //打开房间宝箱
    static void openRoomChest(Player& player, Room& curRoom);
    //收集宝石
    static bool collectGem(const std::string& gemName);
    //检查是否集齐8颗宝石通关
    static bool checkWinCondition();

private:
    static std::vector<std::string> gemList;
};

#endif