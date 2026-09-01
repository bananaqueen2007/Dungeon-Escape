#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include<string>
#include<vector>

//命令解析：把输入字符串分割成 命令 + 参数
struct CommandResult
{
    std::string cmd; //指令动词 look / get / equip
    std::string arg; //参数，例如 get 生命药水 → arg=生命药水
};

class CommandParser
{
public:
    static CommandResult parse(const std::string& input);
};

#endif