#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderWindow;
}

class ValueBar
{
public:
    ValueBar();
    ~ValueBar() = default;

    void draw(sf::RenderWindow& _window);

    void configure(const sf::Vector2f& _size, const float _y_offset,
        const sf::Color& _color = sf::Color::White);

    void setBarColor(const sf::Color& _color);

    void updatePosition(const sf::Vector2f& _position);
    void updateValuePercentage(const float _percentage);

private:
    void init();

    sf::RectangleShape rectangle;
    sf::Vector2f size;
    float y_offset;

};
