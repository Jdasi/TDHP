#pragma once

#include <string>

#include "ProjectileStats.h"

namespace sf
{
    class Texture;
}

struct TowerType
{
    TowerType()
        : slug()
        , texture(nullptr)
        , projectile_slug()
        , engage_radius(0)
        , shot_delay(0)
        , projectile_stats()
    {
    }

    std::string slug;
    sf::Texture* texture;
    std::string projectile_slug;
    float engage_radius;
    float shot_delay;

    ProjectileStats projectile_stats;
};
