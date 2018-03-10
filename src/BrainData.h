#pragma once

struct WorkingKnowledge;
class BrainActionManager;
class BrainStateVisualiser;

struct BrainData
{
    BrainData(WorkingKnowledge& _knowledge, BrainActionManager& _action_manager,
        BrainStateVisualiser& _visualiser)
        : knowledge(_knowledge)
        , action_manager(_action_manager)
        , visualiser(_visualiser)
    {
    }

    WorkingKnowledge& knowledge;
    BrainActionManager& action_manager;
    BrainStateVisualiser& visualiser;

};
