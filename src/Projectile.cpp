#include "Projectile.h"
#include "JTime.h"


Projectile::Projectile()
    : active_duration(0)
    , draw_until_time(0)
    , owning_type(nullptr)
{
}


void Projectile::tick()
{
    // Base class has no behaviour ..
}


void Projectile::setActiveDuration(const float _active_duration)
{
    active_duration = _active_duration;
}


TowerType* Projectile::getOwningType()
{
    return owning_type;
}


void Projectile::setOwningType(TowerType* _owning_type)
{
    owning_type = _owning_type;
}


// Refreshes the projectile visualisation and informs the derived class.
void Projectile::refresh(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    if (owning_type == nullptr)
        return;

    draw_until_time = JTime::getTime() + active_duration;

    onRefresh(_from, _to);
}


bool Projectile::isActive() const
{
    if (JTime::getTime() >= draw_until_time)
        return false;

    return true;
}


// Forces the visualisation to expire.
void Projectile::expire()
{
    draw_until_time = 0;
}
