#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Tower.h"
#include "JHelper.h"
#include "JMath.h"
#include "Constants.h"
#include "Enemy.h"
#include "ProjectileRequest.h"


Tower::Tower()
    : iprojectile_manager(nullptr)
    , type(nullptr)
    , last_shot_timestamp(0)
    , engage_radius(0)
    , engage_radius_sqr(0)
{
    initEngageRadius();
}


void Tower::init(IProjectileManager& _iprojectile_manager)
{
    iprojectile_manager = &_iprojectile_manager;
}


void Tower::setType(TowerType& _type)
{
    type = &_type;

    setTexture(_type.texture);
    updateEngageRadius();
}


void Tower::tick(GameData& _gd)
{
    Enemy* current_target = evaluateCurrentTarget();
    if (current_target != nullptr)
    {
        engage(current_target);
    }
}


void Tower::draw(sf::RenderWindow& _window)
{
    _window.draw(engage_radius_display);

    TDSprite::draw(_window);
}


bool Tower::canShoot() const
{
    return JTime::getTime() > last_shot_timestamp + type->shot_delay;
}


void Tower::updateNearbyEnemies(const std::vector<Enemy*>& _enemies)
{
    nearby_enemies = _enemies;
}


float Tower::getEngageRadiusSqr() const
{
    return engage_radius_sqr;
}


void Tower::onSetPosition()
{
    engage_radius_display.setPosition(getPosition());
}


void Tower::onSpawn()
{
    if (type == nullptr)
        killQuiet();

    last_shot_timestamp = JTime::getTime();
    setRotation(0);

    nearby_enemies.clear();
}


void Tower::initEngageRadius()
{
    engage_radius_display.setFillColor(sf::Color::Transparent);
    engage_radius_display.setOutlineColor(sf::Color::Green);
    engage_radius_display.setOutlineThickness(1);
}


void Tower::updateEngageRadius()
{
    // Actual engage radius is based on the grid scale.
    engage_radius = type->engage_radius * ((getTileWidth() + getTileHeight()) / 2);
    engage_radius_sqr = engage_radius * engage_radius;

    engage_radius_display.setRadius(engage_radius);
    JHelper::centerSFOrigin(engage_radius_display);
}


Enemy* Tower::evaluateCurrentTarget()
{
    Enemy* closest_enemy = nullptr;
    float closest_dist = JMath::maxFloat();

    auto& pos = getPosition();

    for (auto* enemy : nearby_enemies)
    {
        float dist = JMath::vector2DistanceSqr(enemy->getPosition(), pos);
        if (dist > closest_dist)
        {
            continue;
        }

        closest_enemy = enemy;
        closest_dist = dist;
    }

    return closest_enemy;
}


void Tower::engage(Enemy* _enemy)
{
    if (_enemy == nullptr)
        return;

    // Look at the enemy.
    setRotation(JHelper::calculateLookAngle(getPosition(), _enemy->getPosition()));

    if (!canShoot())
        return;

    shoot(_enemy);
}


void Tower::shoot(Enemy* _enemy)
{
    if (_enemy == nullptr)
        return;

    last_shot_timestamp = JTime::getTime();

    if (iprojectile_manager != nullptr)
    {
        iprojectile_manager->requestProjectile({ type, getPosition(), _enemy });
    }
}
