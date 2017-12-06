#include "Enemy.h"
#include "JTime.h"
#include "Constants.h"


Enemy::Enemy()
{

}


void Enemy::tick()
{
    // DEBUG.
    auto& pos = getPosition();
    setPosition(pos.x + (ENEMY_MOVE_SPEED * JTime::getDeltaTime()), pos.y); // Debug
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
