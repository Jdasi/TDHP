#pragma once

#include <string>

namespace sf
{
    class Texture;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct which contains information about the configuration of an Enemy unit.

---------------------------------------------------------*/
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
