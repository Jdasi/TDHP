#pragma once

#include "StateHandler.h"
#include "BrainState.h"
#include "BrainStateType.h"

namespace sf
{
    class RenderWindow;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Specialised version of the StateHandler class, for use with splitting
up the different states of the DirectorBrain.

---------------------------------------------------------*/
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
