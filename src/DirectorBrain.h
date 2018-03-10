#pragma once

#include <vector>
#include <memory>

#include "Scheduler.h"
#include "WorkingKnowledge.h"
#include "EnemySpawn.h"
#include "BrainData.h"
#include "BrainStatistics.h"
#include "BrainStateHandler.h"
#include "BrainStateVisualiser.h"
#include "EnemyListener.h"

class HeatmapManager;
class EnemyManager;
class Level;
struct EnemyType;
struct GameData;

class DirectorBrain : public EnemyListener
{
public:
    DirectorBrain(GameData& _game_data, HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
        std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns, Level& _level);
    ~DirectorBrain();

    void tick();
    void draw(sf::RenderWindow& _window);

    float getEnergyPercentage() const;

private:
    void init();
    void initWorkingKnowledge();
    void initStateSystem();

    void decisionPoint();
    void updateWorkingKnowledge();
    void printDecisionPointLog();
    void makeDecision();

    float heatmapWeightToPercentage(const int _intensity);

    // Energy Tiers.
    void processEnergyTierTwo();
    void processEnergyTierThree();

    // Conditionals.
    bool laserIntensityOverThreshold() const;
    bool bulletIntensityOverThreshold() const;
    bool overallIntensityOverThreshold() const;

    bool fastEnemiesOverThreshold() const;
    bool strongEnemiesOverThreshold() const;
    bool totalEnemiesOverThreshold() const;
    bool enemyCloseToGoal() const;

    // Action Tiers.
    bool tierTwoActions();
    bool tierThreeActions();

    // Actions.
    void noAction();
    void waitingForEnergy();

    void sendFastSwarm();
    void sendStrongSwarm();
    void sendBasicSwarm();

    void healthBoostFastEnemies();
    void healthBoostStrongEnemies();
    void healthBoostAllEnemies();

    void speedBoostFastEnemies();
    void speedBoostStrongEnemies();
    void speedBoostAllEnemies();

    void smokeBomb();

    // Helper.
    void sendSwarm(EnemyType* _type, const int _count,
        const EnemySpawn::SpawnPathType& _path_type);

    // Enemy events.
    void onDeath(const Enemy& _caller, TowerType* _killer_type) override;
    void onPathComplete(Enemy& _caller) override;

    GameData& gd;

    HeatmapManager& heatmap_manager;
    EnemyManager& enemy_manager;
    std::vector<std::unique_ptr<EnemySpawn>>& enemy_spawns;
    Level& level;

    Scheduler scheduler;
    WorkingKnowledge knowledge;
    BrainStatistics statistics;

    std::unique_ptr<BrainData> brain_data;
    std::unique_ptr<BrainStateHandler> state_handler;
    std::unique_ptr<BrainStateVisualiser> state_visualiser;

    float start_time;

};
