#pragma once

struct TargetingPrefs
{
    TargetingPrefs()
        : max_health_factor(0)
        , speed_factor(0)
        , self_distance_factor(0)
        , base_distance_factor(0)
    {
    }

    float max_health_factor;
    float speed_factor;

    float self_distance_factor;
    float base_distance_factor;
};
