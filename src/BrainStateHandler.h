#pragma once

#include "StateHandler.h"
#include "BrainState.h"
#include "BrainStateType.h"

namespace sf
{
    class RenderWindow;
}

class BrainStateHandler final : public StateHandler<BrainState, BrainStateHandler>
{
public:
    BrainStateHandler() = default;
    virtual ~BrainStateHandler() = default;

    void onDecisionPoint()
    {
        current_state->makeDecision();
    }

};
