#include "Killable.h"
#include "TowerType.h"
#include "ProjectileStats.h"


Killable::Killable()
    : max_health(1)
    , health(0)
{
}


int Killable::getMaxHealth() const
{
    return max_health;
}


void Killable::setMaxHealth(const int _max_health)
{
    float prev_health_percentage = getHealthPercentage();

    max_health = _max_health;
    health = static_cast<int>(_max_health * prev_health_percentage);
}


bool Killable::isAlive() const
{
    return health > 0;
}


float Killable::getHealthPercentage() const
{
    return static_cast<float>(health) / static_cast<float>(max_health);
}


// Marks the entity as alive and informs the derived class.
void Killable::spawn()
{
    if (isAlive())
        return;

    health = max_health;
    onSpawn();
}


// Damages the entity and informs the derived class.
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


// Kills the entity and informs the derived class.
void Killable::kill(TowerType* _killer_type)
{
    killQuiet();
    onDeath(_killer_type);
}


// Kills the entity without informing the derived class.
void Killable::killQuiet()
{
    if (!isAlive())
        return;

    health = 0;
}
