#pragma once

#include <vector>
#include <map>
#include <functional>
#include <memory>

#include "BrainAction.h"
#include "BrainActionTypes.h"
#include "EnemySpawn.h"

struct GameData;
struct WorkingKnowledge;
struct BrainStatistics;
class EnemyManager;
class HeatmapManager;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class which registers all of the actions available to the DirectorBrain,
as well as the Preconditions to test against.

The DirectorBrain states can then utilise this class to process their own
sets of actions, which provide different capabilities.

---------------------------------------------------------*/
class BrainActionManager final
{
public:
    BrainActionManager(GameData& _gd, WorkingKnowledge& _knowledge, BrainStatistics& _statistics,
        HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
        std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns);

    ~BrainActionManager() = default;

    void processActionSet(const int _state_key);

private:
    void init();
    void initPreconditions();
    void initActions();
    void initActionSets();

    void addAction(const Brain::ActionType& _type, const int _cost, const std::function<void()>& _action,
        const std::vector<Brain::PreconditionType>& _preconditions);

    void sortActionSet(std::vector<BrainAction*>& _action_set);

    void sendSwarm(EnemyType* _type, const int _count,
        const EnemySpawn::SpawnPathType& _path_type);

    GameData& gd;
    WorkingKnowledge& knowledge;
    BrainStatistics& statistics;
    HeatmapManager& heatmap_manager;
    EnemyManager& enemy_manager;

    std::vector<std::unique_ptr<EnemySpawn>>& enemy_spawns;

    std::map<Brain::PreconditionType, std::function<bool(WorkingKnowledge&)>> precondition_bank;
    std::map<Brain::ActionType, BrainAction> action_bank;
    std::map<int, std::vector<BrainAction*>> action_sets;

};
