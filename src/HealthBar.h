#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderWindow;
}

class HealthBar
{
public:
    HealthBar();
    ~HealthBar() = default;

    void draw(sf::RenderWindow& _window);

    void configure(const sf::Vector2f& _size, const float _y_offset);
    void updatePosition(const sf::Vector2f& _position);
    void updateHealthPercentage(const float _percentage);

private:
    void init();

    sf::RectangleShape rectangle;
    sf::Vector2f size;
    float y_offset;

};
