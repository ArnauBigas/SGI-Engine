#ifndef TIMER_H
#define	TIMER_H

class Timer {
public:
    void start();
    void stop();
    void reset();
    unsigned long getTime();
private:
    bool running;
    unsigned long startms;
    unsigned long ms;
};

#endif	/* TIMER_H */

