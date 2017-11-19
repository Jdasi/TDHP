#include "Enemy.h"
#include "JTime.h"
#include "Constants.h"


Enemy::Enemy()
    : alive(false)
{
}


void Enemy::tick()
{
    // Move towards nav destination?
    auto& pos = getPosition();
    setPosition(pos.x + (ENEMY_MOVE_SPEED * JTime::getDeltaTime()), pos.y); // Debug
}


bool Enemy::isAlive() const
{
    return alive;
}


void Enemy::spawn()
{
    if (alive)
        return;

    alive = true;
}


void Enemy::kill()
{
    if (!alive)
        return;

    alive = false;
    
    for (auto& listener : listeners)
    {
        listener->onDeath(getPosition());
    }
}
