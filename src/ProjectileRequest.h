#pragma once

#include <SFML/System/Vector2.hpp>

struct TowerType;
class Enemy;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct containing information about a request for a Projectile
from a Tower.

---------------------------------------------------------*/
struct ProjectileRequest
{
    ProjectileRequest()
        : tower_type(nullptr)
        , tower_target(nullptr)
    {
    }

    ProjectileRequest(TowerType* _tower_type, const sf::Vector2f& _tower_pos,
        Enemy* _tower_target)
        : tower_type(_tower_type)
        , tower_pos(_tower_pos)
        , tower_target(_tower_target)
    {
    }

    TowerType* tower_type;
    sf::Vector2f tower_pos;
    Enemy* tower_target;
};
