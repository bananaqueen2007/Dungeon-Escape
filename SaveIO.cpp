#include "SaveIO.h"
#pragma once
#include "Player.h"
#include "Room.h"
#include"ChestStory.h"
#include <vector>
#include <memory>   // shared_ptr
#include <fstream>
#include <iostream>

bool SaveIO::saveToFile(const Player& player, const std::vector<std::shared_ptr<Room>>& roomList)
{
    std::ofstream outFile("save.txt");
    if (!outFile.is_open())
    {
        std::cout << "存档失败！无法打开save.txt" << std::endl;
        return false;
    }
    //角色基础属性
    outFile << player.name << std::endl;
    outFile << player.hp << " " << player.maxHp << std::endl;
    outFile << player.baseAtk << " " << player.totalAtk << std::endl;
    outFile << player.gold << std::endl;
    outFile << player.currentRoomId << std::endl;
    outFile << player.hasBoneKey << std::endl;
    outFile << player.tempAtkBuff << " " << player.tempBuffTurn << " " << player.poisonTurn << std::endl;

    //宝石列表
    outFile << ChestStory::gemList.size() << std::endl;
    for (auto& g : ChestStory::gemList) outFile << g << std::endl;

    //背包
    outFile << player.backpack.size() << std::endl;
    for (auto& it : player.backpack)
    {
        outFile << it->name << "|" << it->desc << "|" << it->type << "|" << it->atkBonus << "|" << it->stackCount << std::endl;
    }
    //皮肤列表
    outFile << player.skinList.size() << std::endl;
    for (auto& it : player.skinList)
    {
        outFile << it->name << "|" << it->desc << "|" << it->type << "|" << it->atkBonus << "|" << it->stackCount << std::endl;
    }

    //房间宝箱打开状态
    outFile << roomList.size() << std::endl;
    for (auto& r : roomList)
    {
        outFile << r->id << " " << r->chestOpened << " " << r->locked << std::endl;
    }

    outFile.close();
    std::cout << "?存档成功，已写入save.txt" << std::endl;
    return true;
}

bool SaveIO::loadFromFile(Player& player, std::vector<std::shared_ptr<Room>>& roomList)
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
    inFile >> player.hasBoneKey;
    inFile >> player.tempAtkBuff >> player.tempBuffTurn >> player.poisonTurn;

    int gemCnt;
    inFile >> gemCnt;
    ChestStory::gemList.clear();
    for (int i = 0; i < gemCnt; i++)
    {
        std::string g; inFile >> g;
        ChestStory::gemList.push_back(g);
    }

    //读取背包
    int bpCnt; inFile >> bpCnt;
    player.backpack.clear();
    inFile.ignore();
    for (int i = 0; i < bpCnt; i++)
    {
        std::string line;
        std::getline(inFile, line);
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        std::string name = line.substr(0, p1);
        std::string desc = line.substr(p1 + 1, p2 - p1 - 1);
        std::string type = line.substr(p2 + 1, p3 - p2 - 1);
        int atk = std::stoi(line.substr(p3 + 1, p4 - p3 - 1));
        int stack = std::stoi(line.substr(p4 + 1));
        auto item = std::make_shared<Item>(name, desc, type, atk);
        item->stackCount = stack;
        player.backpack.push_back(item);
    }

    //读取皮肤
    int skinCnt; inFile >> skinCnt;
    inFile.ignore();
    player.skinList.clear();
    for (int i = 0; i < skinCnt; i++)
    {
        std::string line;
        std::getline(inFile, line);
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        std::string name = line.substr(0, p1);
        std::string desc = line.substr(p1 + 1, p2 - p1 - 1);
        std::string type = line.substr(p2 + 1, p3 - p2 - 1);
        int atk = std::stoi(line.substr(p3 + 1, p4 - p3 - 1));
        int stack = std::stoi(line.substr(p4 + 1));
        auto item = std::make_shared<Item>(name, desc, type, atk);
        item->stackCount = stack;
        player.skinList.push_back(item);
    }

    //房间宝箱上锁状态
    int roomCnt; inFile >> roomCnt;
    for (int i = 0; i < roomCnt; i++)
    {
        int rid, chest, lock;
        inFile >> rid >> chest >> lock;
        auto it = std::find_if(roomList.begin(), roomList.end(), [&](std::shared_ptr<Room>& r) {return r->id == rid; });
        if (it != roomList.end())
        {
            (*it)->chestOpened = (chest == 1);
            (*it)->locked = (lock == 1);
        }
    }

    inFile.close();
    std::cout << "?读档成功！欢迎回来，" << player.name << std::endl;
    return true;
}