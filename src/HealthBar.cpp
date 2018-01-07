#include <SFML/Graphics/RenderWindow.hpp>

#include "HealthBar.h"
#include "JHelper.h"


HealthBar::HealthBar()
    : y_offset(0)
{
    init();
}


void HealthBar::draw(sf::RenderWindow& _window)
{
    _window.draw(rectangle);
}


void HealthBar::configure(const sf::Vector2f& _size, const float _y_offset)
{
    size = _size;
    y_offset = _y_offset;

    rectangle.setSize(_size);
    JHelper::centerSFOrigin(rectangle);
}


void HealthBar::updatePosition(const sf::Vector2f& _position)
{
    rectangle.setPosition({ _position.x, _position.y + y_offset });
}


void HealthBar::updateHealthPercentage(const float _percentage)
{
    rectangle.setSize({ size.x * _percentage, size.y });
}


void HealthBar::init()
{
    rectangle.setFillColor(sf::Color::Green);
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(1);
}
