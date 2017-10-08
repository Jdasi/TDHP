#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
    reset();
}


void SimpleTimer::reset()
{
    start = std::chrono::system_clock::now();
}


float SimpleTimer::get_time_difference() const
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = now - start;

    return elapsed_seconds.count();
}


