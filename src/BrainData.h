#pragma once

struct WorkingKnowledge;
class BrainActionManager;
class BrainStateVisualiser;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct to hold references to core DirectorBrain systems used in 
the different brain states.

---------------------------------------------------------*/
struct BrainData final
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
