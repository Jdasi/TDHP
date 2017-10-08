#pragma once

#include "SimpleTimer.h"


class JTime
{
public:
    static float get_unscaled_delta_time();
    static float get_unscaled_time();
    
    static float get_delta_time();
    static float get_time();

    static float get_time_scale();
    static void set_time_scale(float _time_scale);

    void tick();

private:
    static float unscaled_delta_time;
    static float unscaled_time;

    static float delta_time;
    static float time;

    static float time_scale;

    SimpleTimer timer;

};