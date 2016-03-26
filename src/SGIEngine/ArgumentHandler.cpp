#include "ArgumentHandler.h"
#include <algorithm>
#include <string>
#include <vector>

std::vector<std::string> vals;

void ArgHandler::submit(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        vals.push_back(std::string(argv[i]));
    }
}

bool ArgHandler::hasArg(std::string arg) {
    return std::find(vals.begin(), vals.end(), arg) != vals.end();
}

std::vector<std::string> ArgHandler::getValues(std::string arg) {
    std::vector<std::string> ret;
    int i = std::find(vals.begin(), vals.end(), arg) - vals.begin()+1;
    while (i < vals.size() && vals.at(i)[0] != '-') {
        ret.push_back(vals.at(i));
        i++;
    }
    return ret;
}

bool ArgHandler::hasValues(std::string arg) {
    return getValues(arg).size() > 0;
}