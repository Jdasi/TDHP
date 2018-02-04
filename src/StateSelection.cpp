#include "StateSelection.h"
#include "GameData.h"
#include "InputHandler.h"
#include "StateHandler.h"
#include "JTime.h"


StateSelection::StateSelection(GameData& _game_data)
    : State(_game_data)
{
}


void StateSelection::onStateEnter()
{
}


void StateSelection::onStateLeave()
{
}


void StateSelection::tick()
{
    if (gameData().input.getKeyDown(sf::Keyboard::Escape))
        gameData().exit = true;

    if (gameData().input.getKeyDown(sf::Keyboard::Return))
        getHandler()->queueState(GameState::GAME);
}


void StateSelection::draw(sf::RenderWindow& _window)
{
}
