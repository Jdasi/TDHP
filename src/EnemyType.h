#pragma once

#include <string>

namespace sf
{
    class Texture;
}

struct EnemyType
{
    EnemyType()
        : slug()
        , texture(nullptr)
        , speed(0)
        , max_health(0)
    {
    }

    std::string slug;
    sf::Texture* texture;
    float speed;
    int max_health;
};
