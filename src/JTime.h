#pragma once

#include "SimpleTimer.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Static class for easy access to information related to program time.

- Scaled time relates to simulated game time.
- Unscaled time relates to real world time.

JTime's tick function should be called only once during a game loop iteration.

---------------------------------------------------------*/
class JTime final
{
public:
    JTime() = delete;

    static float getUnscaledDeltaTime();
    static float getUnscaledTime();
    
    static float getDeltaTime();
    static float getTime();

    static float getTimeScale();
    static void setTimeScale(float _time_scale);

    static void tick();

private:
    static float unscaled_delta_time;
    static float unscaled_time;

    static float delta_time;
    static float time;

    static float time_scale;

    static SimpleTimer timer;

};
