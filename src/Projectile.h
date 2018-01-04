#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderWindow;
    class Color;
}

class Projectile
{
public:
    Projectile();

    virtual void tick();
    virtual void draw(sf::RenderWindow& _window) = 0;

    void setActiveDuration(const float _active_duration);
    virtual void setColor(const sf::Color& _color) = 0;

    void refresh(const sf::Vector2f& _from, const sf::Vector2f& _to);
    bool isActive() const;

    void destroy();

protected:
    virtual void onRefresh(const sf::Vector2f& _from, const sf::Vector2f& _to) = 0;

private:
    float active_duration;
    float draw_until_time;

};
