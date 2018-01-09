#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "WeightedEnemy.h"
#include "TargetingPrefs.h"

class Enemy;

class TargetingSystem
{
public:
    TargetingSystem();
    ~TargetingSystem() = default;

    Enemy* evaluateClosestTarget(std::vector<Enemy*>& _enemies,
        const sf::Vector2f& _tower_pos);

    Enemy* evaluateBestTarget(std::vector<Enemy*>& _enemies, TargetingPrefs& _targeting_prefs,
        const sf::Vector2f& _tower_pos, const sf::Vector2f& _base_pos);

private:
    void sortEnemies(std::vector<Enemy*>& _enemies, const sf::Vector2f& _tower_pos,
        const sf::Vector2f& _base_pos, TargetingPrefs& targeting_prefs);

    void addDistanceWeight(WeightedEnemy& _we, const sf::Vector2f& _pos,
        const float _factor);

    std::vector<WeightedEnemy> sorted_enemies;

};
