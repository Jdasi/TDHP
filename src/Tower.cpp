#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Tower.h"
#include "JHelper.h"
#include "Constants.h"
#include "Enemy.h"


Tower::Tower()
    : last_shot_timestamp(0)
{
    initEngageRadiusDisplay();
}


void Tower::draw(sf::RenderWindow& _window)
{
    TDSprite::draw(_window);

    _window.draw(engage_radius_display);
    laser.draw(_window);
}


bool Tower::canShoot() const
{
    return JTime::getTime() > last_shot_timestamp + TOWER_SHOT_DELAY;
}


void Tower::shoot(Enemy* _enemy)
{
    if (!_enemy)
        return;

    last_shot_timestamp = JTime::getTime();
    laser.refresh(getPosition(), _enemy->getPosition());

    _enemy->kill();
}


void Tower::onSetPosition()
{
    engage_radius_display.setPosition(getPosition());
}


void Tower::onSpawn()
{
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


void Tower::initEngageRadiusDisplay()
{
    engage_radius_display.setRadius(TOWER_ENGAGE_RADIUS);
    engage_radius_display.setFillColor(sf::Color::Transparent);
    engage_radius_display.setOutlineColor(sf::Color::Green);
    engage_radius_display.setOutlineThickness(1);

    JHelper::centerSFOrigin(engage_radius_display);
}
