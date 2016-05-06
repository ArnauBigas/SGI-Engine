/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Logger.h
 * Author: arnaubigas
 *
 * Created on January 7, 2016, 1:23 PM
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <streambuf>
#include <ostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace Logger {
    
    class LoggerBuffer : public std::streambuf {
    public:
        LoggerBuffer(std::streambuf * output);
    private:
        virtual int overflow(int c);
        virtual int sync();

        std::streambuf * output;
    };
    
    class LoggerStream : public std::ostream{
    public:
        LoggerStream(std::ostream& output, const char * severity);
        
        std::ostream& operator<<(std::string s){
            return (*this << s.c_str());
        }
        
        template <typename T>
        std::ostream& operator<<(const T& data){
            time_t now = time(0);
            struct tm tstruct = *localtime(&now);
            char buf[9];
            strftime(buf, sizeof(buf), "%X", &tstruct);
            
            std::ostringstream buffer;
            buffer << std::dec << "[" << buf << "][" << severity << "]: " << data;
            static_cast<std::ostream&>(*this) << buffer.str();
            return *this;
        }
        
    private:
        LoggerBuffer buffer;
        const char * severity;
    };
    
    
    extern LoggerStream info;
    extern LoggerStream warning;
    extern LoggerStream error;
}

#endif /* LOGGER_H */

