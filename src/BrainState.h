#pragma once

#include "State.h"

namespace sf
{
    class RenderWindow;
}

struct BrainData;
class BrainStateHandler;

class BrainState : public State<BrainState, BrainStateHandler>
{
public:
    explicit BrainState(BrainData& _bd)
        : bd(_bd)
    {
    }

    virtual ~BrainState() = default;

    void onStateEnter() override = 0;
    void onStateLeave() override = 0;

    void tick() override = 0;

    virtual void makeDecision() = 0;

protected:
    BrainData& brainData() const
    {
        return bd;
    }

private:
    BrainData& bd;

};
