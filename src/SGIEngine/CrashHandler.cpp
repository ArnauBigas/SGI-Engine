/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CrashHandler.cpp
 * Author: arnaubigas
 * 
 * Created on January 8, 2016, 3:35 PM
 */

#include "CrashHandler.h"

#include <csignal>
#include <SDL2/SDL_messagebox.h>
#include <map>
#include <sstream>
#include <cstring>
#include <stdarg.h>

#include "definitions.h"

#if defined LINUX
#include <execinfo.h>
#include <cxxabi.h>
#endif

#include "Logger.h"
#include "RenderEngine.h"
#include "Game.h"

std::map<int, std::string> names;

void handleSignal(int signum) {
    if(signum == 2){
        CrashHandler::crash("Application termination requested.");
    } else {
        CrashHandler::crash("Received interrupt signal %i (%s).", signum, names[signum].c_str());
    }
}

void addSignal(int signum, std::string name){
    names.insert(std::pair<int, std::string>(signum, name));
    signal(signum, handleSignal);
}

#define ADDSIGNAL(X) addSignal(X, #X)

namespace CrashHandler {
    void init(){
        // standard signals
        ADDSIGNAL(SIGABRT);
        ADDSIGNAL(SIGFPE);
        ADDSIGNAL(SIGILL);
        ADDSIGNAL(SIGINT);
        ADDSIGNAL(SIGSEGV);
        ADDSIGNAL(SIGTERM);
    }
    
    void crash(const char * format, ...){
        Game::kill();
        
        char buff[MAXERRORLENGTH];
        va_list args;
        va_start(args, format);
        vsnprintf(buff, MAXERRORLENGTH, format, args);
        va_end(args);
        Logger::error << "Application crashed!" << std::endl;
        Logger::error << buff << std::endl;
        
        //Get a backtrace
        #if defined LINUX
            void *array[STACKDEPTH];
            size_t size = backtrace(array, STACKDEPTH);
            char** strings = backtrace_symbols(array, size);
            
            std::ostringstream output;
            
            output << "Backtrace:" << std::endl;
            
            for(size_t i = 0; i < size; i++){
                size_t sz = 128; // just a guess, template names will go much wider
                char *function = static_cast<char*>(malloc(sz));
                char *begin = 0, *end = 0;
                // find the parentheses and address offset surrounding the mangled name
                for (char *j = strings[i]; *j; ++j) {
                    if (*j == '(') {
                        begin = j;
                    }
                    else if (*j == '+') {
                        end = j;
                    }
                }
                if (begin && end) {
                    *begin++ = 0;
                    *end = 0;
                    // found our mangled name, now in [begin, end)

                    int status;
                    char *ret = abi::__cxa_demangle(begin, function, &sz, &status);
                    if (ret) {
                        // return value may be a realloc() of the input
                        function = ret;
                    } else {
                        // demangling failed, just pretend it's a C function with no args
                        std::strncpy(function, begin, sz);
                        std::strncat(function, "()", sz);
                        function[sz-1] = 0;
                    }
                    output << "#" << function << std::endl;
                } else {
                    // didn't find the mangled name, just print the whole line
                    output << strings[i] << std::endl;
                }
                free(function);
            }
            
            Logger::error << output.str();
        #else
            Logger::error << "Couldn't generate a backtrace for this platform." << std::endl;
        #endif
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Ermagherd game crashed you idiot!", (std::string(buff)+std::string("\nA crash report has been generated and saved.")).c_str(), RenderEngine::getWindow());
        exit(0);
    }
}