#pragma once

#include <string>

namespace sf
{
    class Texture;
}

struct EnemyType
{
    EnemyType()
        : texture(nullptr)
        , move_speed(0)
        , max_health(1)
    {
    }

    EnemyType(const std::string& _slug, sf::Texture* _texture,
        const float _speed, const int _health)
        : slug(_slug)
        , texture(_texture)
        , move_speed(_speed)
        , max_health(_health)
    {
    }

    std::string slug;
    sf::Texture* texture;
    float move_speed;
    int max_health;
};
