#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include<string>
using namespace std;

struct CommandResult
{
    string cmd;
    string arg;
};

class CommandParser
{
public:
    static CommandResult parse(const string& input);
};

#endif