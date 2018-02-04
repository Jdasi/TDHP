#pragma once

#include <vector>
#include <memory>
#include <queue>

#include "State.h"

enum class GameState
{
    SELECTION,
    GAME
};

/* Handles all of the State transitions in the game.
 */
class StateHandler
{
public:
    StateHandler();
    virtual ~StateHandler() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void registerState(GameState _game_state, std::unique_ptr<State> _state);
    void queueState(const GameState _state);

private:
    void triggerState(const GameState _state);

    std::vector<std::pair<GameState, std::unique_ptr<State>>> states;
    std::queue<GameState> state_queue;

    State* current_state;
};
