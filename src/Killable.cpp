#include "Killable.h"


Killable::Killable(): max_health(1)
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


void Killable::spawn()
{
    if (isAlive())
        return;

    health = max_health;
    onSpawn();
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
