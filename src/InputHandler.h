#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace sf
{
    class RenderWindow;
    class Event;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Processes SFML events to record differences in key and mouse
states during the core game loop.

Typical usage:

if (input.getKeyDown(sf::Keyboard::Key::A))
{
    // Do something.
}

The lateTick() function should be called at the end of
the game loop to correctly store previous input states.

---------------------------------------------------------*/
class InputHandler final
{
public:
    InputHandler(sf::RenderWindow* _window);
    ~InputHandler() = default;

    void processEvent(sf::Event& _event);
    void lateTick();

    bool getKey(const sf::Keyboard::Key& _key) const;
    bool getKeyDown(const sf::Keyboard::Key& _key) const;
    bool getKeyUp(const sf::Keyboard::Key& _key) const;

    sf::Vector2f getMousePos() const;
    bool getMouseButton(const sf::Mouse::Button& _button) const;
    bool getMouseButtonDown(const sf::Mouse::Button& _button) const;
    bool getMouseButtonUp(const sf::Mouse::Button& _button) const;

private:
    sf::RenderWindow* window;

    bool keys[sf::Keyboard::KeyCount];
    bool prev_keys[sf::Keyboard::KeyCount];

    bool buttons[sf::Mouse::ButtonCount];
    bool prev_buttons[sf::Mouse::ButtonCount];

};
