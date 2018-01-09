#pragma once

class Enemy;

struct WeightedEnemy
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
