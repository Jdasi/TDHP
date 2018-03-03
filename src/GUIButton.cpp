#include <SFML/Graphics/RenderWindow.hpp>

#include "GUIButton.h"
#include "JHelper.h"
#include "GameData.h"
#include "InputHandler.h"


GUIButton::GUIButton()
    : hover(false)
    , pressed(false)
{
}


GUIButton::GUIButton(const sf::Color& _normal, const sf::Color& _hover, const sf::Color& _press)
    : hover(false)
    , pressed(false)
{
    setColors(_normal, _hover, _press);
}


void GUIButton::tick(GameData& _gd)
{
    handleClick(_gd);
    handleColor();
}


void GUIButton::draw(sf::RenderWindow& _window)
{
    _window.draw(shape);
}


void GUIButton::setTexture(sf::Texture* _texture)
{
    shape.setTexture(_texture);
    JHelper::centerSFOrigin(shape);
}


void GUIButton::setColors(const sf::Color& _normal, const sf::Color& _hover, const sf::Color& _press)
{
    normal_color = _normal;
    hover_color = _hover;
    pressed_color = _press;
}


const sf::Vector2f& GUIButton::getPosition() const
{
    return shape.getPosition();
}


void GUIButton::setPosition(const sf::Vector2f& _pos)
{
    shape.setPosition(_pos);
}


void GUIButton::setPosition(const float _x, const float _y)
{
    setPosition(sf::Vector2f(_x, _y));
}


const sf::Vector2f& GUIButton::getSize() const
{
    return shape.getSize();
}


void GUIButton::setSize(const sf::Vector2f& _size)
{
    shape.setSize(_size);
    JHelper::centerSFOrigin(shape);
}


const sf::Vector2f& GUIButton::getScale() const
{
    return shape.getScale();
}


void GUIButton::setScale(const sf::Vector2f& _factors)
{
    shape.setScale(_factors);
    JHelper::centerSFOrigin(shape);
}


float GUIButton::getRotation() const
{
    return shape.getRotation();
}


void GUIButton::setRotation(const float _angle)
{
    shape.setRotation(_angle);
}


void GUIButton::addClickEvent(const std::function<void()>& _function)
{
    click_events.push_back(_function);
}


void GUIButton::clearClickEvents()
{
    click_events.clear();
}


void GUIButton::handleClick(GameData& _gd)
{
    hover = shape.getGlobalBounds().contains(_gd.input.getMousePos());

    if (hover && _gd.input.getMouseButtonDown(sf::Mouse::Left))
    {
        pressed = true;
    }
    else if (_gd.input.getMouseButtonUp(sf::Mouse::Left))
    {
        if (hover && pressed)
        {
            onClick();
        }

        pressed = false;
    }
}


void GUIButton::handleColor()
{
    if (pressed)
    {
        shape.setFillColor(pressed_color);
    }
    else if (hover)
    {
        shape.setFillColor(hover_color);
    }
    else
    {
        shape.setFillColor(normal_color);
    }
}


void GUIButton::onClick()
{
    for (auto& ev : click_events)
    {
        ev();
    }
}
