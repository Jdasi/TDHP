#include "Enemy.h"
#include "JTime.h"
#include "JMath.h"
#include "Constants.h"


Enemy::Enemy()
    : path_index(0)
{
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
        dir *= ENEMY_MOVE_SPEED * JTime::getDeltaTime();

        setPosition(pos + dir);
    }
}


void Enemy::setPath(const LevelPath& _path)
{
    path_index = 0;
    path = _path;
}


void Enemy::onSpawn()
{

}


void Enemy::onDeath()
{
    for (auto& listener : listeners)
    {
        listener->onDeath(getPosition());
    }
}
