#pragma once

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct which contains information about the targeting preferences
of a Tower.

# max_health_factor
Determines the Tower's willingness to engage targets with lots of health.
    - A positive number favours stronger enemies.
    - A negative number favours weaker enemies.

# speed_factor
Determines the Tower's willingness to engage targets with a fast speed.
    - A positive number favours faster enemies.
    - A negative number favours slower enemies.

# self_distance_factor
Determines the Tower's willingness to engage targets distant from itself.
    - A positive number favours distant enemies.
    - A negative number favours closer enemies.

# base_distance_factor
Determines the Tower's willingness to engage targets distant from the base.
    - A positive number favours enemies far from the base.
    - A negative number favours enemies close to the base.

---------------------------------------------------------*/
struct TargetingPrefs final
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
