#pragma once

struct WorkingKnowledge
{
    WorkingKnowledge()
        : hm_total_weight(0)
        , hm_laser_weight(0)
        , hm_bullet_weight(0)
        , total_enemies(0)
        , fast_enemies(0)
        , strong_enemies(0)
        , basic_enemies(0)
        , enemies_queued(0)
        , avg_path_diff(0)
        , cheapest_path(0)
    {
    }

    int hm_total_weight;
    int hm_laser_weight;
    int hm_bullet_weight;

    int total_enemies;
    int fast_enemies;
    int strong_enemies;
    int basic_enemies;
    bool enemies_queued;

    int avg_path_diff;
    int cheapest_path;
};
