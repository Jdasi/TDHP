#pragma once

#include "StateHandler.h"
#include "GameState.h"
#include "GameStateType.h"

namespace sf
{
    class RenderWindow;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Specialised version of the StateHandler class, for use with splitting
up the main parts of the game.

---------------------------------------------------------*/
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
