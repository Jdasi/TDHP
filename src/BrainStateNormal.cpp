#include <iostream>

#include "BrainStateNormal.h"
#include "BrainStateHandler.h"
#include "BrainStateVisualiser.h"
#include "BrainData.h"
#include "WorkingKnowledge.h"
#include "Constants.h"
#include "JMath.h"


BrainStateNormal::BrainStateNormal(BrainData& _bd)
    : BrainState(_bd)
{
}


void BrainStateNormal::onStateEnter()
{
    brainData().visualiser.stateChanged(BRAINSTATE_NORMAL);
}


void BrainStateNormal::onStateLeave()
{
}


void BrainStateNormal::tick()
{
    auto& knowledge = brainData().knowledge;

    if (knowledge.energy < MAX_BRAIN_ENERGY)
    {
        knowledge.energy += NORMAL_ENERGY_REGEN * JTime::getDeltaTime();
        knowledge.energy = JMath::clampf(knowledge.energy, 0, MAX_BRAIN_ENERGY);
    }

    if (knowledge.failed_attack_timer >= TIME_BEFORE_ANGRY)
    {
        getHandler()->queueState(BRAINSTATE_ANGRY);
    }
}


void BrainStateNormal::makeDecision()
{
    auto& knowledge = brainData().knowledge;

    std::cout << "StateNormal Decision: ";
}
