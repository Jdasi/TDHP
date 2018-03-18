#pragma once

#include <vector>
#include <functional>

#include "WorkingKnowledge.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct representing an action that the DirectorBrain can take.
An action's preconditions must be met if the action is to be performed.

The DirectorBrain's energy is automatically tested as part of the
precondition checks for the action.

---------------------------------------------------------*/
struct BrainAction final
{
public:
    BrainAction(const int _cost, const std::function<void()>& _action)
        : energy_cost(_cost)
        , action(_action)
    {
    }


    void addPrecondition(const std::function<bool(WorkingKnowledge&)>& _precond)
    {
        preconditions.push_back(_precond);
    }


    /*
    Attempts to perform the associated action.
    Returns true if the action was performed, or false if one or more
    of the action's preconditions were not met.
    */
    bool performAction(WorkingKnowledge& _knowledge)
    {
        if (_knowledge.energy < energy_cost || !allPreconditionsMet(_knowledge))
            return false;

        _knowledge.energy -= energy_cost;

        if (_knowledge.energy < 0)
            _knowledge.energy = 0;

        action();

        return true;
    }


    int getCost() const
    {
        return energy_cost;
    }

private:
    bool allPreconditionsMet(WorkingKnowledge& _knowledge)
    {
        for (auto& cond : preconditions)
        {
            if (!cond(_knowledge))
                return false;
        }

        return true;
    }

    int energy_cost;
    std::vector<std::function<bool(WorkingKnowledge&)>> preconditions;
    std::function<void()> action;

};
