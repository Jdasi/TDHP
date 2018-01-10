#pragma once

struct WorkingKnowledge
{
    WorkingKnowledge()
        : hm_total_weight(0)
        , hm_laser_weight(0)
        , hm_bullet_weight(0)
        , fast_enemies(0)
        , normal_enemies(0)
        , strong_enemies(0)
        , avg_path_diff(0)
        , cheapest_path(0)
    {
    }

    int hm_total_weight;
    int hm_laser_weight;
    int hm_bullet_weight;

    int fast_enemies;
    int normal_enemies;
    int strong_enemies;

    int avg_path_diff;
    int cheapest_path;
};
