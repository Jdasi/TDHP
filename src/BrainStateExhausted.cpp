#include <iostream>

#include "BrainStateExhausted.h"
#include "BrainStateHandler.h"
#include "BrainStateVisualiser.h"
#include "BrainActionManager.h"
#include "BrainData.h"
#include "WorkingKnowledge.h"
#include "Constants.h"
#include "JMath.h"


BrainStateExhausted::BrainStateExhausted(BrainData& _bd)
    : BrainState(_bd)
{
}


void BrainStateExhausted::onStateEnter()
{
    brainData().visualiser.stateChanged(BRAINSTATE_EXHAUSTED);
}


void BrainStateExhausted::onStateLeave()
{
    // Prevent infinitely switching between Exhausted and Angry states.
    brainData().knowledge.failed_attack_timer = 0;
}


void BrainStateExhausted::tick()
{
    auto& knowledge = brainData().knowledge;

    if (knowledge.energy < MAX_BRAIN_ENERGY)
    {
        knowledge.energy += EXHAUSTED_ENERGY_REGEN * JTime::getDeltaTime();
        knowledge.energy = JMath::clampf(knowledge.energy, 0, MAX_BRAIN_ENERGY);
    }

    if (getHandler()->timeInState() >= BRAIN_EXHAUSTED_TIME)
    {
        getHandler()->queueState(BRAINSTATE_NORMAL);
    }
}


void BrainStateExhausted::makeDecision()
{
    std::cout << "StateExhausted Decision: ";
    brainData().action_manager.processActionSet(BRAINSTATE_EXHAUSTED);
}
