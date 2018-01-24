#pragma once

struct WorkingKnowledge
{
    WorkingKnowledge()
        : energy(0)
        , hm_laser_intensity(0)
        , hm_bullet_intensity(0)
        , swarm_threshold(0)
        , total_enemies(0)
        , fast_enemies(0)
        , strong_enemies(0)
        , basic_enemies(0)
        , enemies_queued(0)
        , avg_path_diff(0)
        , cheapest_path(0)
    {
    }

    float energy;

    int hm_maximum_weight;
    float hm_laser_intensity;
    float hm_bullet_intensity;

    float swarm_threshold;

    int total_enemies;
    int fast_enemies;
    int strong_enemies;
    int basic_enemies;
    bool enemies_queued;

    int avg_path_diff;
    int cheapest_path;
};
