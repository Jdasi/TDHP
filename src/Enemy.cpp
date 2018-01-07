#include "Enemy.h"
#include "JTime.h"
#include "JMath.h"
#include "Constants.h"


Enemy::Enemy()
    : path_index(0)
    , type(nullptr)
{
    initHealthBar();
}


void Enemy::setType(EnemyType& _type)
{
    type = &_type;

    setMaxHealth(type->max_health);
    setTexture(_type.texture);
}


void Enemy::tick()
{
    if (path_index >= path.getNumPoints())
        return;

    auto& waypoint = path.getWaypoint(path_index);
    auto& pos = getPosition();

    if (pos == waypoint.pos ||
        JMath::vector2DistanceSqr(pos, waypoint.pos) <= 1)
    {
        ++path_index;

        if (path_index >= path.getNumPoints())
        {
            for (auto& listener : listeners)
            {
                listener->onPathComplete(*this);
            }
        }
    }
    else
    {
        sf::Vector2f dir = JMath::vector2Normalized(waypoint.pos - pos);
        dir *= type->move_speed * JTime::getDeltaTime();

        setPosition(pos + dir);
    }
}


void Enemy::draw(sf::RenderWindow& _window)
{
    TDSprite::draw(_window);

    // TODO: have a way to toggle the visibility of this ..
    path.draw(_window, getPosition(), path_index);

    health_bar.draw(_window);
}


void Enemy::setPath(const LevelPath& _path)
{
    path_index = 0;
    path = _path;
}


void Enemy::onSpawn()
{
    if (type == nullptr)
        killQuiet();

    health_bar.updateHealthPercentage(getHealthPercentage());
}


void Enemy::onDamage(TowerType* _attacker_type)
{
    health_bar.updateHealthPercentage(getHealthPercentage());
}


void Enemy::onDeath(TowerType* _killer_type)
{
    for (auto& listener : listeners)
    {
        listener->onDeath(getPosition(), _killer_type);
    }
}


void Enemy::onSetPosition()
{
    health_bar.updatePosition(getPosition());
}


void Enemy::initHealthBar()
{
    float width = getTileWidth();
    float height = getTileHeight();

    health_bar.configure({ width * 0.9f, height * 0.1f }, -height * 0.5f);
}
