#pragma once

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct which represents information about the current state of the world.

---------------------------------------------------------*/
struct WorkingKnowledge
{
    float energy                = 0;

    int hm_maximum_weight       = 0;
    float hm_overall_intensity  = 0;
    float hm_laser_intensity    = 0;
    float hm_bullet_intensity   = 0;

    float swarm_threshold       = 0;

    int total_enemies           = 0;
    int fast_enemies            = 0;
    int strong_enemies          = 0;
    int basic_enemies           = 0;

    int proximity_to_goal       = 0;
    bool enemies_queued         = 0;

    int avg_path_diff           = 0;
    int cheapest_path           = 0;

    float failed_attack_timer   = 0;
};
