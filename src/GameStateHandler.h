#pragma once

#include "StateHandler.h"
#include "GameState.h"
#include "GameStateType.h"

namespace sf
{
    class RenderWindow;
}

class GameStateHandler final : public StateHandler<GameState, GameStateHandler>
{
public:
    GameStateHandler() = default;
    virtual ~GameStateHandler() = default;

    void draw(sf::RenderWindow& _window) const
    {
        current_state->draw(_window);
    }

};
