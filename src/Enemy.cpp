#include "Enemy.h"
#include "JTime.h"
#include "JMath.h"
#include "Constants.h"


Enemy::Enemy()
    : path_index(0)
    , type(nullptr)
{
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
                listener->onPathComplete(this);
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
    path.draw(_window, getPosition(), path_index);
}


void Enemy::setPath(const LevelPath& _path)
{
    path_index = 0;
    path = _path;
}


void Enemy::onSpawn()
{
    if (!type)
        killQuiet();
}


void Enemy::onDeath()
{
    for (auto& listener : listeners)
    {
        listener->onDeath(getPosition());
    }
}
