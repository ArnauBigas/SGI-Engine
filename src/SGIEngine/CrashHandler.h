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

/**
 The maximum amount of characters as a string that can be passed into a crash reason
 */
#define MAXERRORLENGTH 512

namespace CrashHandler {
    void init();
    
    void crash(const char * format, ...);
};

#endif /* CRASHHANDLER_H */

