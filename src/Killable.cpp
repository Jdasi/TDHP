#include "Killable.h"
#include "TowerType.h"
#include "ProjectileStats.h"


Killable::Killable()
    : max_health(1)
    , health(0)
{
}


void Killable::setMaxHealth(const int _max_health)
{
    max_health = _max_health;
}


bool Killable::isAlive() const
{
    return health > 0;
}


float Killable::getHealthPercentage() const
{
    return static_cast<float>(health) / static_cast<float>(max_health);
}


void Killable::spawn()
{
    if (isAlive())
        return;

    health = max_health;
    onSpawn();
}


void Killable::damage(TowerType* _attacker_type)
{
    if (!isAlive())
        return;

    health -= _attacker_type->projectile_stats.damage;

    if (!isAlive())
    {
        onDeath(_attacker_type);
    }
    else
    {
        onDamage(_attacker_type);
    }
}


void Killable::kill(TowerType* _killer_type)
{
    killQuiet();
    onDeath(_killer_type);
}


void Killable::killQuiet()
{
    if (!isAlive())
        return;

    health = 0;
}
