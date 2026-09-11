#include "CommandParser.h"
#include <sstream>
using namespace std;

CommandResult CommandParser::parse(const string& input)
{
    CommandResult res;
    res.cmd = "";
    res.arg = "";

    istringstream iss(input);
    iss >> res.cmd;

    string remain;
    getline(iss, remain);

    size_t start = remain.find_first_not_of(" \t");
    if (start != string::npos)
    {
        size_t end = remain.find_last_not_of(" \t");
        res.arg = remain.substr(start, end - start + 1);
    }
    return res;
}