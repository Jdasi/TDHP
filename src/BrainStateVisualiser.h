#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

namespace sf
{
    class RenderWindow;
}

class BrainStateVisualiser
{
public:
    BrainStateVisualiser();
    ~BrainStateVisualiser() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void stateChanged(int _state_key);

private:
    void init();

    sf::RectangleShape overlay;

};
