#include <SFML/Graphics/RenderWindow.hpp>

#include "ValueBar.h"
#include "JHelper.h"


ValueBar::ValueBar()
    : y_offset(0)
{
    init();
}


void ValueBar::draw(sf::RenderWindow& _window)
{
    _window.draw(rectangle);
}


void ValueBar::configure(const sf::Vector2f& _size, const float _y_offset,
    const sf::Color& _color)
{
    size = _size;
    y_offset = _y_offset;

    rectangle.setSize(_size);
    JHelper::centerSFOrigin(rectangle);

    setBarColor(_color);
}


void ValueBar::setBarColor(const sf::Color& _color)
{
    rectangle.setFillColor(_color);
}


void ValueBar::updatePosition(const sf::Vector2f& _position)
{
    rectangle.setPosition({ _position.x, _position.y + y_offset });
}


void ValueBar::updateValuePercentage(const float _percentage)
{
    rectangle.setSize({ size.x * _percentage, size.y });
}


void ValueBar::init()
{
    rectangle.setOutlineColor(sf::Color::White);
    rectangle.setOutlineThickness(1);
}
