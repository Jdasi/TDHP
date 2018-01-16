#pragma once

#include <vector>
#include <memory>

#include "Scheduler.h"
#include "WorkingKnowledge.h"

class HeatmapManager;
class EnemySpawn;
class EnemyManager;

class DirectorBrain
{
public:
    DirectorBrain(HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
        std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns);

    ~DirectorBrain() = default;

    void tick();

private:
    void decisionPoint();
    void updateWorkingKnowledge();
    void printDecisionPointLog();

    HeatmapManager& heatmap_manager;
    EnemyManager& enemy_manager;
    std::vector<std::unique_ptr<EnemySpawn>>& enemy_spawns;

    Scheduler scheduler;
    WorkingKnowledge knowledge;

};
