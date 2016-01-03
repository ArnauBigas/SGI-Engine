/* 
 * File:   Profiler.h
 * Author: Sam
 *
 * Created on 03 January 2016, 20:17
 */

#ifndef PROFILER_H
#define	PROFILER_H

#include <vector>
#include <string>
#include "Timer.h"

namespace Profiler {
    struct Profile {
        std::vector<Profile *> children;
        Profile *parent;
        std::string name;
        Timer timer;
        
        Profile *get(std::string name);
        void reset();
        ~Profile();
    };
    
    void init();
    void cleanup();
    void start(std::string name);
    void stop();
    void reset();
    Profile *get(std::string name);
}

#endif	/* PROFILER_H */

