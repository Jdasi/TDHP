#pragma once

#include <SFML/System/Vector2.hpp>

class Enemy;

class EnemyListener
{
public:
    EnemyListener() = default;
    ~EnemyListener() = default;

    virtual void onDeath(const sf::Vector2f& _pos) {}
    virtual void onPathComplete(Enemy* _caller) {}

};
