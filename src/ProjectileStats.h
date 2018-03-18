#pragma once

#include <string>

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct which contains information about the type of projectile
that a Tower fires.

---------------------------------------------------------*/
struct ProjectileStats final
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
