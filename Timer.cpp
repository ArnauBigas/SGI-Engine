#include "Timer.h"

#include "src/SGIEngine/definitions.h"
#include <time.h>

long getMS() {
    long ret = 0;
#ifdef WINDOWS
    ret = clock();
#endif
#ifdef LINUX
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ret = ts.tv_nsec / 1000000 + ts.tv_sec * 1000;
#endif
    return ret;
}

void Timer::start() {
    running = true;
    startms = getMS();
}

void Timer::stop() {
    if (running) ms = getMS();
    running = false;
}

void Timer::reset() {
    ms = startms = getMS();
}

unsigned long Timer::getTime() {
    if (running) ms = getMS();
    return ms - startms;
}