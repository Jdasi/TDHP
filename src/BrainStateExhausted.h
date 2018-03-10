#pragma once

#include "BrainState.h"

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
