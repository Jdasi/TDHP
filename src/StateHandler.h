#pragma once

#include <vector>
#include <memory>

#include "JTime.h"

/* StateHandler uses a self-referring template pattern.
 * This allows for the StateHandler to stay generic, meaning it can be reused
 * in different areas of the program.
 */
template <typename StateType, typename StateHandlerType>
class StateHandler
{
using State = std::pair<int, std::unique_ptr<StateType>>;

public:
    StateHandler()
        : current_state(nullptr)
        , queued_state(nullptr)
        , time_in_state(0)
    {
    }

    virtual ~StateHandler() = default;

    void registerState(int _key, std::unique_ptr<StateType> _state)
    {
        _state->setHandler(this);
        states.emplace_back(_key, std::move(_state));

        std::sort(states.begin(), states.end(),
            [](const State& _lhs, const State& _rhs)
        {
            return _lhs.first < _rhs.first;
        });
    }

    void queueState(int _key)
    {
        auto result = std::find_if(states.begin(), states.end(),
            [_key](const State& _elem)
        {
            return _elem.first == _key;
        });

        if (result == states.end())
            return;

        queued_state = result->second.get();
    }

    void tick()
    {
        time_in_state += JTime::getDeltaTime();
        processQueuedState();

        if (current_state)
        {
            current_state->tick();
        }
    }

    float timeInState() const
    {
        return time_in_state;
    }

protected:
    StateType* current_state;

private:
    void processQueuedState()
    {
        if (queued_state)
        {
            triggerQueuedState();
        }
    }

    void triggerQueuedState()
    {
        if (current_state)
        {
            current_state->onStateLeave();
        }

        current_state = queued_state;
        queued_state = nullptr;

        current_state->onStateEnter();
        time_in_state = 0;
    }

    std::vector<State> states;
    StateType* queued_state;

    float time_in_state;

};
