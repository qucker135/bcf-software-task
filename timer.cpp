#include "timer.hpp"

using std::chrono::duration_cast;
using std::chrono::duration;

void Timer::start()
{
    tp_start = high_resolution_clock::now();
}

void Timer::stop()
{
    tp_stop = high_resolution_clock::now();
}

double Timer::getTime(){
    return duration_cast<duration<double>>(tp_stop-tp_start).count();
}