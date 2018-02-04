#include "StateHandler.h"


StateHandler::StateHandler()
    : current_state(nullptr)
{
}


void StateHandler::tick()
{
    if (!state_queue.empty())
    {
        triggerState(state_queue.back());
        state_queue.pop();
    }

    if (current_state)
    {
        current_state->tick();
    }
}


void StateHandler::draw(sf::RenderWindow& _window)
{
    if (current_state)
    {
        current_state->draw(_window);
    }
}


void StateHandler::registerState(GameState _game_state, std::unique_ptr<State> _state)
{
    _state->setHandler(this);

    states.emplace_back(_game_state, std::move(_state));

    std::sort(states.begin(), states.end(),
        [](const auto& _lhs, const auto& _rhs)
    {
        return _lhs.first < _rhs.first;
    });
}


/* Adds a state transition to the queue.
 * The StateHandler's tick will carry out the transition when possible.
 */
void StateHandler::queueState(const GameState _state)
{
    state_queue.push(_state);
}


/* Tries to find the passed GameState. Throws an exception if it can't.
 * If the GameState is found, the current state changes to the new state.
 * The appropriate onStateLeave and onStateEnter functions are called for each state.
 */
void StateHandler::triggerState(const GameState _state)
{
    auto result = std::find_if(states.begin(), states.end(),
        [_state](const auto& _elem)
    {
        return _elem.first == _state;
    });

    if (result == states.end())
    {
        throw std::runtime_error("Unknown state.");
    }

    if (current_state)
    {
        current_state->onStateLeave();
    }

    current_state = result->second.get();
    current_state->onStateEnter();
}
