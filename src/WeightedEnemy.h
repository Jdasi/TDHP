#pragma once

class Enemy;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct which contains a pointer to an Enemy and a float for
weighting the attractiveness of that Enemy.

Provides a sorting operator for containers.

---------------------------------------------------------*/
struct WeightedEnemy final
{
    WeightedEnemy(Enemy* _enemy)
        : enemy(_enemy)
        , weight(0)
    {
    }

    // Operator is flipped internally so std::sort is in Descending order.
    bool operator<(const WeightedEnemy& _rhs) const
    {
        return weight > _rhs.weight;
    }

    Enemy* enemy;
    float weight;
};
