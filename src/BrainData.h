#pragma once

struct WorkingKnowledge;
class BrainStateVisualiser;

struct BrainData
{
    BrainData(WorkingKnowledge& _knowledge, BrainStateVisualiser& _visualiser)
        : knowledge(_knowledge)
        , visualiser(_visualiser)
    {
    }

    WorkingKnowledge& knowledge;
    BrainStateVisualiser& visualiser;

};
