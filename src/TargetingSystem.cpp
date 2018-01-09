#include <algorithm>

#include "TargetingSystem.h"
#include "JHelper.h"
#include "JMath.h"
#include "Enemy.h"
#include "Constants.h"


TargetingSystem::TargetingSystem()
{
    sorted_enemies.reserve(MAX_ENEMIES);
}


Enemy* TargetingSystem::evaluateClosestTarget(std::vector<Enemy*>& _enemies, const sf::Vector2f& _tower_pos)
{
    Enemy* closest_enemy = nullptr;
    float closest_dist = JMath::maxFloat();

    for (auto* enemy : _enemies)
    {
        float dist = JMath::vector2DistanceSqr(enemy->getPosition(), _tower_pos);
        if (dist > closest_dist)
        {
            continue;
        }

        closest_enemy = enemy;
        closest_dist = dist;
    }

    return closest_enemy;
}


Enemy* TargetingSystem::evaluateBestTarget(std::vector<Enemy*>& _enemies, TargetingPrefs& _targeting_prefs,
    const sf::Vector2f& _tower_pos, const sf::Vector2f& _base_pos)
{
    if (_enemies.empty())
    {
        // No enemies = no best target.
        return nullptr;
    }
    else if (_enemies.size() == 1)
    {
        // No need process vectors with only one enemy.
        return _enemies[0];
    }

    sortEnemies(_enemies, _tower_pos, _base_pos, _targeting_prefs);
    return sorted_enemies[0].enemy;
}


void TargetingSystem::sortEnemies(std::vector<Enemy*>& _enemies, const sf::Vector2f& _tower_pos,
    const sf::Vector2f& _base_pos, TargetingPrefs& targeting_prefs)
{
    // Clear previous data ready for sort.
    sorted_enemies.clear();

    for (auto* enemy : _enemies)
    {
        WeightedEnemy we(enemy);

        we.weight += enemy->getMaxHealth() * targeting_prefs.max_health_factor;
        we.weight += enemy->getType()->speed * targeting_prefs.speed_factor;

        if (targeting_prefs.self_distance_factor != 0)
            addDistanceWeight(we, _tower_pos, targeting_prefs.self_distance_factor);

        if (targeting_prefs.base_distance_factor != 0)
            addDistanceWeight(we, _base_pos, targeting_prefs.base_distance_factor);

        sorted_enemies.push_back(we);
    }

    // Sort based on weight.
    std::sort(sorted_enemies.begin(), sorted_enemies.end());
}


void TargetingSystem::addDistanceWeight(WeightedEnemy& _we, const sf::Vector2f& _pos, const float _factor)
{
    float dist = JMath::vector2Distance(_we.enemy->getPosition(), _pos);
    _we.weight += dist * _factor;
}
