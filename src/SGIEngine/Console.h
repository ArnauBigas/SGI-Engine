/* 
 * File:   Console.h
 * Author: Sam
 *
 * Created on 04 January 2016, 18:09
 */

#ifndef CONSOLE_H
#define	CONSOLE_H

#include <string>
#include <vector>

typedef void (*CommandFunc)(std::vector<std::string>);

namespace Console {
    void registerCommand(std::string cmd, CommandFunc cFunc);
    void call(std::string cmd);
    void print(std::string text);
}

#endif	/* CONSOLE_H */

