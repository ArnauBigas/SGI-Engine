#include "Console.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "Text.h"

std::map<std::string, CommandFunc> cmds;

void Console::registerCommand(std::string cmd, CommandFunc cFunc) {
    cmds[cmd] = cFunc;
}

void Console::call(std::string cmd) {
    std::string buf;
    std::stringstream ss(cmd);

    std::vector<std::string> vals;

    while (ss >> buf) vals.push_back(buf);
    
    std::map<std::string, CommandFunc>::iterator it = cmds.find(vals[0]);
    if (it != cmds.end()) {
        cmds[vals[0]](vals);
    } else {
        print(C_RED "Error: " C_BLACK + vals[0] + C_RED " is not a command");
    }
}
