#include <iostream>
#include <algorithm>

#include "BrainActionManager.h"
#include "BrainStateType.h"
#include "JHelper.h"
#include "JMath.h"
#include "GameData.h"
#include "GameAudio.h"
#include "Constants.h"
#include "EnemyManager.h"
#include "HeatmapManager.h"
#include "BrainStatistics.h"


BrainActionManager::BrainActionManager(GameData& _gd, WorkingKnowledge& _knowledge,
    BrainStatistics& _statistics, HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
    std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns)
    : gd(_gd)
    , knowledge(_knowledge)
    , statistics(_statistics)
    , heatmap_manager(_heatmap_manager)
    , enemy_manager(_enemy_manager)
    , enemy_spawns(_enemy_spawns)
{
    init();
}


void BrainActionManager::processActionSet(const int _state_key)
{
    auto action_set = action_sets.find(_state_key);
    if (action_set == action_sets.end())
        return;

    unsigned int size = action_sets[_state_key].size();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (action_sets[_state_key][i]->performAction(knowledge))
            return;
    }
}


void BrainActionManager::init()
{
    initPreconditions();
    initActions();
    initActionSets();
}


void BrainActionManager::initPreconditions()
{
    precondition_bank.emplace
    (
        Brain::PreconditionType::BULLET_INTENSITY_OVER_THRESHOLD,
        [](WorkingKnowledge& _wk) { return _wk.hm_bullet_intensity > _wk.swarm_threshold; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::LASER_INTENSITY_OVER_THRESHOLD,
        [](WorkingKnowledge& _wk) { return _wk.hm_laser_intensity > _wk.swarm_threshold; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::OVERALL_INTENSITY_OVER_THRESHOLD,
        [](WorkingKnowledge& _wk) { return _wk.hm_overall_intensity > _wk.swarm_threshold * 2; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::FAST_ENEMIES_OVER_THRESHOLD,
        [](WorkingKnowledge& _wk) { return static_cast<float>(_wk.fast_enemies) >= 3; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::STRONG_ENEMIES_OVER_THRESHOLD,
        [](WorkingKnowledge& _wk) { return static_cast<float>(_wk.strong_enemies) >= 5; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::ENEMY_CLOSE_TO_GOAL,
        [](WorkingKnowledge& _wk) { return _wk.proximity_to_goal <= 5; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::BULLET_INTENSITY_GREATER,
        [](WorkingKnowledge& _wk) { return _wk.hm_bullet_intensity > _wk.hm_laser_intensity; }
    );

    precondition_bank.emplace
    (
        Brain::PreconditionType::LASER_INTENSITY_GREATER,
        [](WorkingKnowledge& _wk) { return _wk.hm_laser_intensity > _wk.hm_bullet_intensity; }
    );
}


void BrainActionManager::initActions()
{
    BrainAction* action = nullptr;

    action = addAction(Brain::ActionType::SEND_FAST_SWARM, 50, [this]()
    {
        sendSwarm(enemy_manager.getFastestType(), 3, EnemySpawn::SpawnPathType::INFLUENCED);

        std::cout << "Sending Fast Enemy Swarm";
        ++statistics.fast_swarm_times;
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::BULLET_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::SEND_STRONG_SWARM, 50, [this]()
    {
        sendSwarm(enemy_manager.getStrongestType(), 3, EnemySpawn::SpawnPathType::PURE);

        std::cout << "Sending Strong Enemy Swarm";
        ++statistics.strong_swarm_times;
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::LASER_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::SPEED_BOOST_ALL_ENEMIES, 30, [this]()
    {
        enemy_manager.boostEnemySpeed(1.5f, 3);

        std::cout << "Boosting All Enemy Speed";
        ++statistics.sb_all_times;

        gd.audio.playSound(SPEED_BOOST_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::ENEMY_CLOSE_TO_GOAL));
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::BULLET_INTENSITY_GREATER));

    action = addAction(Brain::ActionType::HEALTH_BOOST_ALL_ENEMIES, 30, [this]()
    {
        enemy_manager.boostEnemyHealth(2, 5);

        std::cout << "Boosting All Enemy Health";
        ++statistics.hb_all_times;

        gd.audio.playSound(HEALTH_BOOST_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::ENEMY_CLOSE_TO_GOAL));
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::LASER_INTENSITY_GREATER));

    action = addAction(Brain::ActionType::HEALTH_BOOST_FAST_ENEMIES, 25, [this]()
    {
        auto type = enemy_manager.getFastestType();
        enemy_manager.boostEnemyHealth(type, 2, 5);

        std::cout << "Boosting Fast Enemy Health";
        ++statistics.hb_fast_times;

        gd.audio.playSound(HEALTH_BOOST_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::FAST_ENEMIES_OVER_THRESHOLD));
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::LASER_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::HEALTH_BOOST_STRONG_ENEMIES, 25, [this]()
    {
        auto type = enemy_manager.getStrongestType();
        enemy_manager.boostEnemyHealth(type, 2, 5);

        std::cout << "Boosting Strong Enemy Health";
        ++statistics.hb_strong_times;

        gd.audio.playSound(HEALTH_BOOST_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::STRONG_ENEMIES_OVER_THRESHOLD));
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::LASER_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::SPEED_BOOST_FAST_ENEMIES, 25, [this]()
    {
        auto type = enemy_manager.getFastestType();
        enemy_manager.boostEnemySpeed(type, 1.5f, 3);

        std::cout << "Boosting Fast Enemy Speed";
        ++statistics.sb_fast_times;

        gd.audio.playSound(SPEED_BOOST_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::FAST_ENEMIES_OVER_THRESHOLD));
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::BULLET_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::SPEED_BOOST_STRONG_ENEMIES, 25, [this]()
    {
        auto type = enemy_manager.getStrongestType();
        enemy_manager.boostEnemySpeed(type, 1.5f, 3);

        std::cout << "Boosting Strong Enemy Speed";
        ++statistics.sb_strong_times;

        gd.audio.playSound(SPEED_BOOST_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::STRONG_ENEMIES_OVER_THRESHOLD));
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::BULLET_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::SMOKE_BOMB, 25, [this]()
    {
        int index = heatmap_manager.getHighestWeightIndex(HeatmapFlag::DIRECTOR);
        heatmap_manager.splashOnHeatmap(HeatmapFlag::SMOKE, index, 5);

        std::cout << "Dropping Smoke Bomb";
        ++statistics.smoke_times;

        gd.audio.playSound(SMOKE_BOMB_SOUND);
    });
    action->addPrecondition(precondition_bank.at(Brain::PreconditionType::OVERALL_INTENSITY_OVER_THRESHOLD));

    action = addAction(Brain::ActionType::NO_ACTION, 0, [this]()
    {
        std::cout << "No Action";
        ++statistics.no_action_times;
    });
}


void BrainActionManager::initActionSets()
{
    action_sets[BRAINSTATE_NORMAL] =
    {
        &action_bank.at(Brain::ActionType::HEALTH_BOOST_ALL_ENEMIES),
        &action_bank.at(Brain::ActionType::HEALTH_BOOST_FAST_ENEMIES),
        &action_bank.at(Brain::ActionType::HEALTH_BOOST_STRONG_ENEMIES),
        &action_bank.at(Brain::ActionType::NO_ACTION),
        &action_bank.at(Brain::ActionType::SEND_FAST_SWARM),
        &action_bank.at(Brain::ActionType::SEND_STRONG_SWARM),
        &action_bank.at(Brain::ActionType::SMOKE_BOMB),
        &action_bank.at(Brain::ActionType::SPEED_BOOST_ALL_ENEMIES),
        &action_bank.at(Brain::ActionType::SPEED_BOOST_FAST_ENEMIES),
        &action_bank.at(Brain::ActionType::SPEED_BOOST_STRONG_ENEMIES),
    };

    action_sets[BRAINSTATE_ANGRY] =
    {
        &action_bank.at(Brain::ActionType::NO_ACTION),
        &action_bank.at(Brain::ActionType::SEND_FAST_SWARM),
        &action_bank.at(Brain::ActionType::SEND_STRONG_SWARM),
    };

    action_sets[BRAINSTATE_EXHAUSTED] =
    {
        &action_bank.at(Brain::ActionType::NO_ACTION),
    };

    sortActionSet(action_sets.at(BRAINSTATE_NORMAL));
    sortActionSet(action_sets.at(BRAINSTATE_ANGRY));
    sortActionSet(action_sets.at(BRAINSTATE_EXHAUSTED));
}


BrainAction* BrainActionManager::addAction(const Brain::ActionType& _type, const int _cost, const std::function<void()>& _action)
{
    if (action_bank.find(_type) != action_bank.end())
        return nullptr;

    action_bank.emplace(_type, BrainAction(_cost, _action));

    return &action_bank.at(_type);
}


// Sorts the action set by action energy cost in descending order.
void BrainActionManager::sortActionSet(std::vector<BrainAction*>& _action_set)
{
    std::sort(_action_set.begin(), _action_set.end(),
        [](BrainAction* _a, BrainAction* _b)
    {
        return _a->getCost() > _b->getCost(); 
    });
}


void BrainActionManager::sendSwarm(EnemyType* _type, const int _count, const EnemySpawn::SpawnPathType& _path_type)
{
    EnemySpawn* best_spawn = nullptr;
    int cheapest_path = JMath::maxInt();

    for (auto& spawn : enemy_spawns)
    {
        int path_cost = spawn->getPathCost();
        if (path_cost >= cheapest_path)
            continue;

        best_spawn = spawn.get();
        cheapest_path = path_cost;
    }

    float spawn_delay = 2 - (_type->speed * 0.02f);
    spawn_delay = JMath::clampf(spawn_delay, 0.5f, 2);

    for (int i = 0; i < _count; ++i)
    {
        best_spawn->queueEnemy(_type, i * spawn_delay, _path_type);
    }

    gd.audio.playSound(SWARM_SOUND);
}
