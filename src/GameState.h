#pragma once

#include "State.h"

namespace sf
{
    class RenderWindow;
}

struct GameData;
class GameStateHandler;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Specialised version of the State class, for use with splitting
up the main parts of the game.

---------------------------------------------------------*/
class GameState : public State<GameState, GameStateHandler>
{
public:
    explicit GameState(GameData& _gd)
        : gd(_gd)
    {
    }

    virtual ~GameState() = default;

    void onStateEnter() override = 0;
    void onStateLeave() override = 0;

    void tick() override = 0;
    virtual void draw(sf::RenderWindow& _window) = 0;

protected:
    GameData& gameData() const
    {
        return gd;
    }

private:
    GameData& gd;

};
