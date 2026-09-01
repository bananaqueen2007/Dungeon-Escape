#include "SaveIO.h"
#include <iostream>

bool SaveIO::saveToFile(const Player& player)
{
    std::ofstream outFile("save.txt");
    if (!outFile.is_open())
    {
        std::cout << "存档失败！无法打开save.txt" << std::endl;
        return false;
    }

    //基础角色属性
    outFile << player.name << std::endl;
    outFile << player.hp << " " << player.maxHp << std::endl;
    outFile << player.baseAtk << " " << player.totalAtk << std::endl;
    outFile << player.gold << std::endl;
    outFile << player.currentRoomId << std::endl;

    //这里简化：装备、背包、皮肤，联调阶段扩展完整写入逻辑
    outFile.close();
    std::cout << "?存档成功，已写入save.txt" << std::endl;
    return true;
}

bool SaveIO::loadFromFile(Player& player)
{
    std::ifstream inFile("save.txt");
    if (!inFile.is_open())
    {
        std::cout << "?读档失败，未找到save.txt存档文件！" << std::endl;
        return false;
    }

    inFile >> player.name;
    inFile >> player.hp >> player.maxHp;
    inFile >> player.baseAtk >> player.totalAtk;
    inFile >> player.gold;
    inFile >> player.currentRoomId;

    inFile.close();
    std::cout << "?读档成功！欢迎回来，" << player.name << std::endl;
    return true;
}

