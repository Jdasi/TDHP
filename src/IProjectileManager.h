#pragma once

#include <SFML/System/Vector2.hpp>

class IProjectileManager
{
public:
    IProjectileManager() = default;
    ~IProjectileManager() = default;

    virtual void requestLaser(const sf::Vector2f& _from, const sf::Vector2f& _to) {}
    virtual void requestBullet(const sf::Vector2f& _from, const sf::Vector2f& _to) {}

};
