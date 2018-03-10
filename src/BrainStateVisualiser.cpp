#include <SFML/Graphics/RenderWindow.hpp>

#include "BrainStateVisualiser.h"
#include "BrainStateType.h"
#include "Constants.h"


BrainStateVisualiser::BrainStateVisualiser()
{
    init();
}


void BrainStateVisualiser::tick()
{
}


void BrainStateVisualiser::draw(sf::RenderWindow& _window)
{
    _window.draw(overlay);
}


void BrainStateVisualiser::stateChanged(int _state_key)
{
    switch (_state_key)
    {
        case BRAINSTATE_NORMAL:
        {
            overlay.setFillColor(sf::Color::Transparent);
        } break;

        case BRAINSTATE_ANGRY:
        {
            overlay.setFillColor(sf::Color(255, 0, 0, 15));
        } break;

        case BRAINSTATE_EXHAUSTED:
        {
            overlay.setFillColor(sf::Color(0, 0, 255, 15));
        } break;
    }
}


void BrainStateVisualiser::init()
{
    overlay.setSize(WINDOW_SIZE);
}
