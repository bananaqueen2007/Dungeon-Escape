#include "SaveIO.h"
#include "Player.h"
#include "Room.h"
#include"ChestStory.h"
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

bool SaveIO::saveToFile(const Player& player, const std::vector<std::shared_ptr<Room>>& roomList)
{
    ofstream outFile("save.txt");
    if (!outFile.is_open())
    {
        cout << "存档失败！无法打开save.txt" << endl;
        return false;
    }
    outFile << player.name << endl;
    outFile << player.hp << " " << player.maxHp << endl;
    outFile << player.baseAtk << " " << player.totalAtk << endl;
    outFile << player.gold << endl;
    outFile << player.currentRoomId << endl;
    outFile << player.hasBoneKey << endl;
    outFile << player.tempAtkBuff << " " << player.tempBuffTurn << " " << player.poisonTurn << endl;

    outFile << ChestStory::gemList.size() << endl;
    for (auto& g : ChestStory::gemList) outFile << g << endl;

    outFile << player.backpack.size() << endl;
    for (auto& it : player.backpack)
    {
        outFile << it->name << "|" << it->desc << "|" << it->type << "|" << it->atkBonus << "|" << it->stackCount << endl;
    }

    outFile << roomList.size() << endl;
    for (auto& r : roomList)
    {
        outFile << r->id << " " << r->chestOpened << " " << r->locked << " " << r->hasChest << endl;
    }

    outFile.close();
    cout << "存档成功，已写入save.txt" << endl;
    return true;
}

bool SaveIO::loadFromFile(Player& player, std::vector<std::shared_ptr<Room>>& roomList)
{
    ifstream inFile("save.txt");
    if (!inFile.is_open())
    {
        cout << "读档失败，未找到save.txt存档文件！" << endl;
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
        string g; inFile >> g;
        ChestStory::gemList.push_back(g);
    }

    int bpCnt; inFile >> bpCnt;
    player.backpack.clear();
    inFile.ignore();
    for (int i = 0; i < bpCnt; i++)
    {
        string line;
        getline(inFile, line);
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        string name = line.substr(0, p1);
        string desc = line.substr(p1 + 1, p2 - p1 - 1);
        string type = line.substr(p2 + 1, p3 - p2 - 1);
        int atk = stoi(line.substr(p3 + 1, p4 - p3 - 1));
        int stack = stoi(line.substr(p4 + 1));
        auto item = make_shared<Item>(name, desc, type, atk);
        item->stackCount = stack;
        player.backpack.push_back(item);
    }

    int roomCnt; inFile >> roomCnt;
    for (int i = 0; i < roomCnt; i++)
    {
        int rid, chest, lock, hasch;
        inFile >> rid >> chest >> lock >> hasch;
        auto it = find_if(roomList.begin(), roomList.end(), [&](shared_ptr<Room>& r) {return r->id == rid; });
        if (it != roomList.end())
        {
            (*it)->chestOpened = (chest == 1);
            (*it)->locked = (lock == 1);
            (*it)->hasChest = (hasch == 1);
        }
    }

    inFile.close();
    cout << "读档成功！欢迎回来，" << player.name << endl;
    return true;
}