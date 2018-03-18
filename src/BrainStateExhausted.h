#pragma once

#include "BrainState.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

The state that represents the DirectorBrain's capabilities
when it is exhausted.

---------------------------------------------------------*/
class BrainStateExhausted final : public BrainState
{
public:
    explicit BrainStateExhausted(BrainData& _bd);
    virtual ~BrainStateExhausted() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;

    void makeDecision() override;

private:


};
