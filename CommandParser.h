#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include<string>

struct CommandResult
{
    std::string cmd;
    std::string arg;
};

class CommandParser
{
public:
    static CommandResult parse(const std::string& input);
};

#endif
