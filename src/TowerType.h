#pragma once

#include <string>

#include "ProjectileStats.h"
#include "TargetingPrefs.h"

namespace sf
{
    class Texture;
}

struct TowerType
{
    TowerType()
        : slug()
        , texture(nullptr)
        , engage_radius(0)
        , shot_delay(0)
        , projectile_stats()
        , targeting_prefs()
    {
    }

    std::string slug;
    sf::Texture* texture;
    float engage_radius;
    float shot_delay;

    ProjectileStats projectile_stats;
    TargetingPrefs targeting_prefs;
};
