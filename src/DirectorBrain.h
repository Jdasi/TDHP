#pragma once

#include <vector>

#include "Scheduler.h"
#include "WorkingKnowledge.h"

class HeatmapManager;
class EnemySpawn;
class EnemyTypeManager;

class DirectorBrain
{
public:
    DirectorBrain(HeatmapManager& _heatmap_manager, EnemyTypeManager& _enemy_type_manager,
        std::vector<EnemySpawn>& _enemy_spawns);

    ~DirectorBrain() = default;

    void tick();

private:
    void decisionPoint();
    void updateWorkingKnowledge();
    void printDecisionPointLog();

    HeatmapManager& heatmap_manager;
    EnemyTypeManager& enemy_type_manager;
    std::vector<EnemySpawn>& enemy_spawns;

    Scheduler scheduler;
    WorkingKnowledge knowledge;

};
