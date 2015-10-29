/* 
 * File:   ArgumentHandler.h
 * Author: Sam
 *
 * Created on 27 October 2015, 16:58
 */

#ifndef ARGUMENTHANDLER_H
#define	ARGUMENTHANDLER_H

#include <vector>
#include <string>

namespace ArgHandler {
    void submit(int argc, char** argv);
    bool hasArg(std::string arg);
    std::vector<std::string> getValues(std::string arg);
    bool hasValues(std::string arg);
}

#endif	/* ARGUMENTHANDLER_H */

