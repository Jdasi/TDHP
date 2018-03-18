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
    _window.draw(bg_rectangle);
    _window.draw(fg_rectangle);
}


// Sets the bar's settings. Automatically centers the bar's origin.
void ValueBar::configure(const sf::Vector2f& _size, const float _y_offset,
    const sf::Color& _color)
{
    size = _size;
    y_offset = _y_offset;

    bg_rectangle.setSize(_size);
    fg_rectangle.setSize(_size);

    JHelper::centerSFOrigin(bg_rectangle);
    JHelper::centerSFOrigin(fg_rectangle);

    setBarColor(_color);
}


void ValueBar::setBarColor(const sf::Color& _color)
{
    fg_rectangle.setFillColor(_color);
}


// Moves the bar to a new position. Automatically factors in its offset.
void ValueBar::updatePosition(const sf::Vector2f& _position)
{
    bg_rectangle.setPosition({ _position.x, _position.y + y_offset });
    fg_rectangle.setPosition({ _position.x, _position.y + y_offset });
}


// Updates the foreground fill of the bar. Value should be between 0-1.
void ValueBar::updateValuePercentage(const float _percentage)
{
    fg_rectangle.setSize({ size.x * _percentage, size.y });
}


void ValueBar::init()
{
    bg_rectangle.setOutlineColor(sf::Color::White);
    bg_rectangle.setOutlineThickness(1);
    bg_rectangle.setFillColor(sf::Color::Black);
}
