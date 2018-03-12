#include <iostream>
#include <iomanip>

#include "DirectorBrain.h"
#include "HeatmapManager.h"
#include "EnemyManager.h"
#include "JTime.h"
#include "JMath.h"
#include "Constants.h"
#include "FileIO.h"

#include "BrainStateNormal.h"
#include "BrainStateAngry.h"
#include "BrainStateExhausted.h"


DirectorBrain::DirectorBrain(GameData& _gd, HeatmapManager& _heatmap_manager,
    EnemyManager& _enemy_manager, std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns, Level& _level)
    : gd(_gd)
    , heatmap_manager(_heatmap_manager)
    , enemy_manager(_enemy_manager)
    , enemy_spawns(_enemy_spawns)
    , level(_level)
    , action_manager(_gd, knowledge, statistics, _heatmap_manager, _enemy_manager, _enemy_spawns)
    , start_time(JTime::getTime())
{
    init();
}


DirectorBrain::~DirectorBrain()
{
    // Output brain session statistics to file.
    statistics.session_duration = JTime::getTime() - start_time;
    FileIO::exportBrainStatistics(statistics);
}


void DirectorBrain::tick()
{
    scheduler.update();

    state_handler->tick();
    state_visualiser->tick();

    knowledge.failed_attack_timer += JTime::getDeltaTime();
}


void DirectorBrain::draw(sf::RenderWindow& _window)
{
    state_visualiser->draw(_window);
}


float DirectorBrain::getEnergyPercentage() const
{
    return knowledge.energy / MAX_BRAIN_ENERGY;
}


void DirectorBrain::init()
{
    enemy_manager.addEnemyListener(this);
    statistics.level_name = level.getName();

    scheduler.invokeRepeating([this]()
    {
        decisionPoint();
    }, 5.0f, 5.0f);

    state_visualiser = std::make_unique<BrainStateVisualiser>(gd);
    brain_data = std::make_unique<BrainData>(knowledge, action_manager, *state_visualiser.get());

    initWorkingKnowledge();
    initStateSystem();
}


void DirectorBrain::initWorkingKnowledge()
{
    knowledge.hm_maximum_weight = level.getProduct() * static_cast<int>(MAX_WEIGHTING);

    /* Math to scale swarm threshold based on level size.
     * E.g. a larger map should have a lower swarm threshold.
     */
    float max_int = static_cast<float>(JMath::maxInt());
    knowledge.swarm_threshold = (max_int / knowledge.hm_maximum_weight) * 0.0002f;
}


void DirectorBrain::initStateSystem()
{
    state_handler = std::make_unique<BrainStateHandler>();

    state_handler->registerState(BRAINSTATE_NORMAL, std::make_unique<BrainStateNormal>(*brain_data.get()));
    state_handler->registerState(BRAINSTATE_ANGRY, std::make_unique<BrainStateAngry>(*brain_data.get()));
    state_handler->registerState(BRAINSTATE_EXHAUSTED, std::make_unique<BrainStateExhausted>(*brain_data.get()));

    state_handler->queueState(BRAINSTATE_NORMAL);
}


void DirectorBrain::decisionPoint()
{
    updateWorkingKnowledge();

    std::cout << "----------------------------------------------------------\n";
    printDecisionPointLog();
    std::cout << "----------------------------------------------------------\n";
    makeDecision();
    std::cout << "\n----------------------------------------------------------\n";

    ++statistics.decision_points;
}


void DirectorBrain::updateWorkingKnowledge()
{
    int overall_weight = heatmap_manager.getTotalWeight();
    knowledge.hm_overall_intensity = heatmapWeightToPercentage(overall_weight);

    int laser_weight = heatmap_manager.getTotalWeight(HeatmapFlag::LASER_DEATHS, true);
    knowledge.hm_laser_intensity = heatmapWeightToPercentage(laser_weight);

    int bullet_weight = heatmap_manager.getTotalWeight(HeatmapFlag::BULLET_DEATHS, true);
    knowledge.hm_bullet_intensity = heatmapWeightToPercentage(bullet_weight);

    knowledge.avg_path_diff = 0;
    knowledge.cheapest_path = JMath::maxInt();

    knowledge.total_enemies = enemy_manager.getNumAlive();
    knowledge.fast_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getFastestType());
    knowledge.strong_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getStrongestType());
    knowledge.basic_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getBasicType());

    knowledge.proximity_to_goal = enemy_manager.getProximityToGoal();
    knowledge.enemies_queued = false;

    for (auto& spawn : enemy_spawns)
    {
        if (spawn->enemiesInQueue())
            knowledge.enemies_queued = true;

        knowledge.avg_path_diff += spawn->getPathDifference();

        int path_cost = spawn->getPathCost();
        if (path_cost > knowledge.cheapest_path)
            continue;

        knowledge.cheapest_path = path_cost;
    }

    if (!enemy_spawns.empty())
    {
        knowledge.avg_path_diff /= enemy_spawns.size();
    }
}


void DirectorBrain::printDecisionPointLog()
{
    std::string enemies_queued_output(JHelper::boolToStr(knowledge.enemies_queued));

    std::cout
        << std::setprecision(3)
        << "[Brain Decision Point at " << JTime::getTime() << "s with " << knowledge.energy << " energy]\n"
        << '\n'
        << "--HEATMAP INFO:\n"
        << "+ Overall Heatmap Intensity: " << knowledge.hm_overall_intensity << "/100\n"
        << "+   Laser Heatmap Intensity: " << knowledge.hm_laser_intensity   << "/100\n"
        << "+  Bullet Heatmap Intensity: " << knowledge.hm_bullet_intensity  << "/100\n"
        << '\n'
        << "--ENEMY INFO:\n"
        << "+             Total Enemies: " << knowledge.total_enemies        << '\n'
        << "+              Fast Enemies: " << knowledge.fast_enemies         << '\n'
        << "+            Strong Enemies: " << knowledge.strong_enemies       << '\n'
        << "+             Basic Enemies: " << knowledge.basic_enemies        << '\n'
        << '\n'
        << "+         Proximity to Goal: " << knowledge.proximity_to_goal    << '\n'
        << "+            Enemies Queued: " << enemies_queued_output          << '\n'
        << '\n'
        << "--PATH INFO:\n"
        << "+        Cheapest Path Cost: " << knowledge.cheapest_path        << '\n'
        << "+   Average Path Difference: " << knowledge.avg_path_diff        << '\n';
}


void DirectorBrain::makeDecision()
{
    state_handler->onDecisionPoint();
}


float DirectorBrain::heatmapWeightToPercentage(const int _weight)
{
    return (static_cast<float>(_weight) / knowledge.hm_maximum_weight) * 100;
}


void DirectorBrain::onDeath(const Enemy& _caller, TowerType* _killer_type)
{
    int tile_index = JHelper::posToTileIndex(_caller.getPosition(), level);

    if (_killer_type != nullptr)
    {
        if (_killer_type->slug == LASER_TOWER_SLUG)
        {
            heatmap_manager.splashOnHeatmap(HeatmapFlag::LASER_DEATHS, tile_index, 3);
        }
        else if (_killer_type->slug == BULLET_TOWER_SLUG)
        {
            heatmap_manager.splashOnHeatmap(HeatmapFlag::BULLET_DEATHS, tile_index, 3);
        }
    }

    knowledge.energy += ENERGY_ON_DEATH;
    knowledge.energy = JMath::clampf(knowledge.energy, 0, MAX_BRAIN_ENERGY);
}


void DirectorBrain::onPathComplete(Enemy& _caller)
{
    _caller.killQuiet();

    knowledge.energy += ENERGY_ON_PATH_COMPLETION;
    knowledge.energy = JMath::clampf(knowledge.energy, 0, MAX_BRAIN_ENERGY);

    knowledge.failed_attack_timer = 0;
    ++statistics.completed_paths;
}
