#pragma once

#include <vector>
#include <memory>

#include "Scheduler.h"
#include "WorkingKnowledge.h"
#include "EnemySpawn.h"
#include "BrainStatistics.h"
#include "EnemyListener.h"

class HeatmapManager;
class EnemySpawn;
class EnemyManager;
class Level;
struct EnemyType;

class DirectorBrain : public EnemyListener
{
public:
    DirectorBrain(HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
        std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns, Level& _level);

    ~DirectorBrain();

    void tick();

    float getEnergyPercentage() const;

private:
    void initWorkingKnowledge();

    void decisionPoint();
    void updateWorkingKnowledge();
    void printDecisionPointLog();
    void makeDecision();

    float heatmapWeightToPercentage(const int _intensity);

    // Energy Tiers.
    void processEnergyTierOne();
    void processEnergyTierTwo();
    void processEnergyTierThree();

    // Conditionals.
    bool laserIntensityOverThreshold() const;
    bool bulletIntensityOverThreshold() const;
    bool overallIntensityOverThreshold() const;

    bool fastEnemiesOverThreshold() const;
    bool strongEnemiesOverThreshold() const;
    bool totalEnemiesOverThreshold() const;

    // Action Tiers.
    bool tierOneActions();
    bool tierTwoActions();
    bool tierThreeActions();

    // Actions.
    void noAction();

    void sendFastSwarm();
    void sendStrongSwarm();
    void sendBasicSwarm();

    void healthBoostFastEnemies();
    void healthBoostStrongEnemies();
    void healthBoostAllEnemies();

    void speedBoostFastEnemies();
    void speedBoostStrongEnemies();
    void speedBoostAllEnemies();

    // Helper.
    void sendSwarm(EnemyType* _type, const int _count, const EnemySpawn::SpawnPathType& _path);

    // Enemy events.
    void onDeath(const sf::Vector2f& _pos, TowerType* _killer_type) override;
    void onPathComplete(Enemy& _caller) override;

    HeatmapManager& heatmap_manager;
    EnemyManager& enemy_manager;
    std::vector<std::unique_ptr<EnemySpawn>>& enemy_spawns;
    Level& level;

    Scheduler scheduler;
    WorkingKnowledge knowledge;
    BrainStatistics statistics;

};
