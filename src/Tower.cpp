#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Tower.h"
#include "JHelper.h"
#include "Constants.h"
#include "Enemy.h"


Tower::Tower()
    : last_shot_timestamp(0)
    , shot_delay(1)
    , engage_radius(0)
    , engage_radius_sqr(0)
{
    initEngageRadius();
}


void Tower::draw(sf::RenderWindow& _window)
{
    _window.draw(engage_radius_display);
    laser.draw(_window);

    TDSprite::draw(_window);
}


bool Tower::canShoot() const
{
    return JTime::getTime() > last_shot_timestamp + shot_delay;
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
    if (!_enemy)
        return;

    last_shot_timestamp = JTime::getTime();
    laser.refresh(getPosition(), _enemy->getPosition());

    _enemy->kill();
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
    last_shot_timestamp = JTime::getTime();
}


void Tower::onDeath()
{
}


Tower::TowerLaser::TowerLaser()
    : line(sf::LineStrip, 2)
    , line_color(sf::Color::Cyan)
    , visible_duration(0.1f)
    , draw_until_time(0)
{
    line[0].color = line_color;
    line[1].color = line_color;
}


void Tower::TowerLaser::draw(sf::RenderWindow& _window)
{
    if (JTime::getTime() > draw_until_time)
        return;

    _window.draw(line);
}


void Tower::TowerLaser::refresh(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    line[0].position = _from;
    line[1].position = _to;

    draw_until_time = JTime::getTime() + visible_duration;
}


void Tower::initEngageRadius()
{
    // Engage radius is based on the grid scale.
    engage_radius = 4/*4=magic number to be replaced later*/ * ((getTileWidth() + getTileHeight()) / 2);
    engage_radius_sqr = engage_radius * engage_radius;

    engage_radius_display.setRadius(engage_radius);
    engage_radius_display.setFillColor(sf::Color::Transparent);
    engage_radius_display.setOutlineColor(sf::Color::Green);
    engage_radius_display.setOutlineThickness(1);

    JHelper::centerSFOrigin(engage_radius_display);
}
