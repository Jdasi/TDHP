#include <iostream>

#include "Tower.h"
#include "JTime.h"
#include "JHelper.h"
#include "Constants.h"
#include "Enemy.h"


Tower::Tower(const int _tile_index)
    : tile_index(_tile_index)
    , last_shot_timestamp(0)
{
    initEngageRadiusDisplay();
}


void Tower::draw(sf::RenderWindow& _window)
{
    _window.draw(tower_sprite);
    _window.draw(engage_radius_display);
}


int Tower::getTileIndex() const
{
    return tile_index;
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
    _enemy->kill();
}


void Tower::setTexture(sf::Texture* _texture)
{
    tower_sprite.setTexture(*_texture);
}


const sf::Vector2f& Tower::getPosition() const
{
    return tower_sprite.getPosition();
}


void Tower::setPosition(const sf::Vector2f& _position)
{
    tower_sprite.setPosition(_position);
    engage_radius_display.setPosition(_position);
}


void Tower::setScale(const sf::Vector2f& _factors)
{
    tower_sprite.setScale(_factors);

    JHelper::centerSFOrigin(tower_sprite);
    JHelper::centerSFOrigin(engage_radius_display);
}


void Tower::setScale(const float _x, const float _y)
{
    setScale(sf::Vector2f(_x, _y));
}


void Tower::initEngageRadiusDisplay()
{
    engage_radius_display.setRadius(TOWER_ENGAGE_RADIUS);
    engage_radius_display.setFillColor(sf::Color::Transparent);
    engage_radius_display.setOutlineColor(sf::Color::Green);
    engage_radius_display.setOutlineThickness(1);
}
