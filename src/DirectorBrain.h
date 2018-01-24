#pragma once

#include <vector>
#include <memory>

#include "Scheduler.h"
#include "WorkingKnowledge.h"

class HeatmapManager;
class EnemySpawn;
class EnemyManager;
class Level;

class DirectorBrain
{
public:
    DirectorBrain(HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
        std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns, Level& _level);

    ~DirectorBrain() = default;

    void tick();

private:
    void initWorkingKnowledge();

    void decisionPoint();
    void updateWorkingKnowledge();
    void printDecisionPointLog();
    void makeDecision();

    float heatmapWeightToPercentage(const int _intensity);

    HeatmapManager& heatmap_manager;
    EnemyManager& enemy_manager;
    std::vector<std::unique_ptr<EnemySpawn>>& enemy_spawns;
    Level& level;

    Scheduler scheduler;
    WorkingKnowledge knowledge;

};
