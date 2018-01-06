#include "TowerBullet.h"
#include "JTime.h"
#include "JMath.h"


TowerBullet::TowerBullet()
    : visible_duration(3)
    , draw_until_time(0)
{
}


void TowerBullet::tick()
{
    float dt = JTime::getDeltaTime();
    float move_speed = getOwningType()->projectile_stats.move_speed;

    sprite.setPosition(sprite.getPosition() + (direction * dt * move_speed));
}


void TowerBullet::draw(sf::RenderWindow& _window)
{
    sprite.draw(_window);
}


void TowerBullet::setTexture(sf::Texture* _texture)
{
    sprite.setTexture(_texture);
}


void TowerBullet::setColor(const sf::Color& _color)
{
    sprite.setColor(_color);
}


const sf::Vector2f& TowerBullet::getPosition() const
{
    return sprite.getPosition();
}


void TowerBullet::onRefresh(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    sprite.setPosition(_from);
    direction = JMath::vector2Normalized(_to - _from);
}
