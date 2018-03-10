#include <iostream>

#include "BrainStateAngry.h"
#include "BrainStateHandler.h"
#include "BrainStateVisualiser.h"
#include "BrainData.h"
#include "WorkingKnowledge.h"
#include "Constants.h"
#include "JMath.h"


BrainStateAngry::BrainStateAngry(BrainData& _bd)
    : BrainState(_bd)
{
}


void BrainStateAngry::onStateEnter()
{
    brainData().visualiser.stateChanged(BRAINSTATE_ANGRY);
}


void BrainStateAngry::onStateLeave()
{
}


void BrainStateAngry::tick()
{
    auto& knowledge = brainData().knowledge;

    if (knowledge.energy < MAX_BRAIN_ENERGY)
    {
        knowledge.energy += ANGRY_ENERGY_REGEN * JTime::getDeltaTime();
        knowledge.energy = JMath::clampf(knowledge.energy, 0, MAX_BRAIN_ENERGY);
    }

    if (getHandler()->timeInState() >= BRAIN_ANGRY_TIME)
    {
        getHandler()->queueState(BRAINSTATE_EXHAUSTED);
    }
}


void BrainStateAngry::makeDecision()
{
    auto& knowledge = brainData().knowledge;

    std::cout << "StateAngry Decision: ";
}
