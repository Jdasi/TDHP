#pragma once

#include <SFML/System/Vector2.hpp>

class Enemy;
struct TowerType;

class EnemyListener
{
public:
    EnemyListener() = default;
    ~EnemyListener() = default;

    virtual void onDeath(const sf::Vector2f& _pos, TowerType* _killer_type = nullptr) {}
    virtual void onPathComplete(Enemy& _caller) {}

};
