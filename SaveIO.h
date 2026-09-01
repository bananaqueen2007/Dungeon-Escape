#pragma once
#ifndef SAVEIO_H
#define SAVEIO_H

#include <fstream>
#include "Player.h"

//负责序列化保存玩家全部数据到save.txt；从save.txt反序列化恢复玩家
class SaveIO
{
public:
    //手动存档、quit自动存档，将player全部信息写入save.txt
    static bool saveToFile(const Player& player);

    //读取save.txt，恢复玩家对象；读取失败返回false
    static bool loadFromFile(Player& player);
};

#endif