#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include <chrono>

using std::chrono::high_resolution_clock;

class Timer{
    public:
    void start();
    void stop();
    double getTime();
    
    private:
    high_resolution_clock::time_point tp_start;
    high_resolution_clock::time_point tp_stop;
};

#endif