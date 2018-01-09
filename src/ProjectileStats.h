#pragma once

#include <string>

struct ProjectileStats
{
    ProjectileStats()
        : slug()
        , speed(0)
        , damage(0)
    {
    }

    std::string slug;
    float speed;
    int damage;
};
