#include <SFML/Graphics.hpp>

#include "InputHandler.h"


InputHandler::InputHandler(sf::RenderWindow* _window)
    : window(_window)
{
}


void InputHandler::processEvent(sf::Event& _event)
{
    switch (_event.type)
    {
        case sf::Event::KeyPressed:
        {
            if (_event.key.code == sf::Keyboard::Unknown)
                return;

            keys[_event.key.code] = true;
        } break;

        case sf::Event::KeyReleased:
        {
            if (_event.key.code == sf::Keyboard::Unknown)
                return;

            keys[_event.key.code] = false;
        } break;

        case sf::Event::MouseButtonPressed:
        {
            buttons[_event.mouseButton.button] = true;
        } break;

        case sf::Event::MouseButtonReleased:
        {
            buttons[_event.mouseButton.button] = false;
        } break;
    }
}


void InputHandler::lateTick()
{
    memcpy(prev_keys, keys, sizeof(bool) * sf::Keyboard::KeyCount);
    memcpy(prev_buttons, buttons, sizeof(bool) * sf::Mouse::ButtonCount);
}


bool InputHandler::getKey(const sf::Keyboard::Key& _key) const
{
    return keys[_key];
}


bool InputHandler::getKeyDown(const sf::Keyboard::Key& _key) const
{
    return keys[_key] && !prev_keys[_key];
}


bool InputHandler::getKeyUp(const sf::Keyboard::Key& _key) const
{
    return !keys[_key] && prev_keys[_key];
}


sf::Vector2f InputHandler::getMousePos() const
{
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}


bool InputHandler::getMouseButton(const sf::Mouse::Button& _button) const
{
    return buttons[_button];
}


bool InputHandler::getMouseButtonDown(const sf::Mouse::Button& _button) const
{
    return buttons[_button] && !prev_buttons[_button];
}


bool InputHandler::getMouseButtonUp(const sf::Mouse::Button& _button) const
{
    return !buttons[_button] && prev_buttons[_button];
}
