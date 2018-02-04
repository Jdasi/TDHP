#include "State.h"


State::State(GameData& _game_data)
    : game_data(_game_data)
    , handler(nullptr)
{
}


StateHandler* State::getHandler() const
{
    return handler;
}


GameData& State::gameData() const
{
    return game_data;
}


void State::setHandler(StateHandler* _handler)
{
    handler = _handler;
}