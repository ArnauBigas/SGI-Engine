/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Logger.h"

#include <iostream>
#include <fstream>

std::ofstream logFile("game.log");

namespace Logger {
    
    LoggerBuffer::LoggerBuffer(std::streambuf* output) : output(output) {}
    
    // Not an actual buffer, we just let the other streams buffer themselves.
    int LoggerBuffer::overflow(int c)
    {
        if (c == EOF)
        {
            return !EOF;
        }
        else
        {
            int const r1 = output->sputc(c);
            int const r2 = logFile.rdbuf()->sputc(c);
            return r1 == EOF || r2 == EOF ? EOF : c;
        }
    }
    
    // Sync both teed buffers.
    int LoggerBuffer::sync()
    {
        int const r1 = output->pubsync();
        int const r2 = logFile.rdbuf()->pubsync();
        return r1 == 0 && r2 == 0 ? 0 : -1;
    }  
    
   LoggerStream::LoggerStream(std::ostream& output, const char * severity)
    : buffer(output.rdbuf()), severity(severity) {this->init(&buffer);}
    
    LoggerStream info(std::cout, "INFO");
    LoggerStream warning(std::cout, "WARNING");
    LoggerStream error(std::cerr, "ERROR");
    
}