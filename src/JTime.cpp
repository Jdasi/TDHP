#include "JTime.h"

float JTime::unscaled_delta_time = 0;
float JTime::unscaled_time = 0;
float JTime::delta_time = 0;
float JTime::time = 0;
float JTime::time_scale = 1;


float JTime::get_unscaled_delta_time()
{
    return unscaled_delta_time;
}


float JTime::get_unscaled_time()
{
    return unscaled_time;
}


float JTime::get_delta_time()
{
    return delta_time;
}


float JTime::get_time()
{
    return time;
}


float JTime::get_time_scale()
{
    return time_scale;
}


void JTime::set_time_scale(float _time_scale)
{
    time_scale = _time_scale;
}



void JTime::tick()
{
    unscaled_delta_time = timer.get_time_difference();
    unscaled_time += JTime::unscaled_delta_time;

    delta_time = JTime::unscaled_delta_time * JTime::time_scale;
    time += JTime::delta_time;

    timer.reset();
}
