#pragma once

#include "State.h"

namespace sf
{
    class RenderWindow;
}

struct GameData;
class GameStateHandler;

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
