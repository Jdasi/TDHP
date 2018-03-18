#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>

#include "TowerType.h"

namespace sf
{
    class RenderWindow;
    class Color;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Base class for all Projectiles that a Tower can fire.
Derived classes must supply a visualisation.

The class automatically hides its visualisation when it has expired.

---------------------------------------------------------*/
class Projectile
{
public:
    Projectile();

    virtual void tick();
    virtual void draw(sf::RenderWindow& _window) = 0;

    void setActiveDuration(const float _active_duration);
    virtual void setColor(const sf::Color& _color) = 0;

    TowerType* getOwningType();
    void setOwningType(TowerType* _owning_type);

    void refresh(const sf::Vector2f& _from, const sf::Vector2f& _to);
    bool isActive() const;

    void expire();

protected:
    virtual void onRefresh(const sf::Vector2f& _from, const sf::Vector2f& _to) = 0;

private:
    float active_duration;
    float draw_until_time;

    TowerType* owning_type;

};
