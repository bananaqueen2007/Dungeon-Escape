#include "CommandParser.h"
#include <sstream>

CommandResult CommandParser::parse(const std::string& input)
{
    CommandResult res;
    res.cmd = "";
    res.arg = "";

    std::istringstream iss(input);
    iss >> res.cmd;

    //读取剩下全部字符串作为参数
    std::string remain;
    std::getline(iss, remain);

    //去除参数前后空格
    size_t start = remain.find_first_not_of(" \t");
    if (start != std::string::npos)
    {
        size_t end = remain.find_last_not_of(" \t");
        res.arg = remain.substr(start, end?start + 1);
    }
    return res;
}