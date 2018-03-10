#pragma once

#include "BrainState.h"

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
