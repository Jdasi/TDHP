#pragma once

#include "BrainState.h"

class BrainStateNormal final : public BrainState
{
public:
    explicit BrainStateNormal(BrainData& _bd);
    virtual ~BrainStateNormal() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;

    void makeDecision() override;

private:


};
