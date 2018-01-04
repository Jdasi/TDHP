#pragma once

#include <string>

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
    {
    }

    TowerType(const std::string& _slug, sf::Texture* _texture,
        const std::string& _projectile_slug, const float _engage_radius,
        const float _shot_delay)
        : slug(_slug)
        , texture(_texture)
        , projectile_slug(_projectile_slug)
        , engage_radius(_engage_radius)
        , shot_delay(_shot_delay)
    {
    }

    std::string slug;
    sf::Texture* texture;
    std::string projectile_slug;
    float engage_radius;
    float shot_delay;
};
