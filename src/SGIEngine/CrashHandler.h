/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CrashHandler.h
 * Author: arnaubigas
 *
 * Created on January 8, 2016, 3:35 PM
 */

#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <string>

#define STACKDEPTH 16

namespace CrashHandler {
    void init();
    
    void crash(std::string reason);
};

#endif /* CRASHHANDLER_H */

