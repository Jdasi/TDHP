#pragma once

#include "BrainState.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

The state that represents the DirectorBrain's capabilities
when it is angry.

---------------------------------------------------------*/
class BrainStateAngry final : public BrainState
{
public:
    explicit BrainStateAngry(BrainData& _bd);
    virtual ~BrainStateAngry() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;

    void makeDecision() override;

private:


};
