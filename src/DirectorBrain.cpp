#include <iostream>
#include <iomanip>

#include "DirectorBrain.h"
#include "HeatmapManager.h"
#include "EnemySpawn.h"
#include "EnemyManager.h"
#include "JTime.h"
#include "JMath.h"
#include "Constants.h"


DirectorBrain::DirectorBrain(HeatmapManager& _heatmap_manager, EnemyManager& _enemy_manager,
    std::vector<std::unique_ptr<EnemySpawn>>& _enemy_spawns, Level& _level)
    : heatmap_manager(_heatmap_manager)
    , enemy_manager(_enemy_manager)
    , enemy_spawns(_enemy_spawns)
    , level(_level)
{
    initWorkingKnowledge();

    scheduler.invokeRepeating([this]()
    {
        decisionPoint();
    }, 5.0f, 5.0f);
}


void DirectorBrain::tick()
{
    scheduler.update();

    if (knowledge.energy < MAX_DIRECTOR_ENERGY)
    {
        knowledge.energy += DIRECTOR_ENERGY_REGEN * JTime::getDeltaTime();
        JMath::clampf(knowledge.energy, 0, MAX_DIRECTOR_ENERGY);
    }
}


float DirectorBrain::getEnergyPercentage() const
{
    return knowledge.energy / MAX_DIRECTOR_ENERGY;
}


void DirectorBrain::initWorkingKnowledge()
{
    knowledge.hm_maximum_weight = level.getProduct() * static_cast<int>(MAX_WEIGHTING);

    // Math to scale swarm threshold based on level size.
    float max_int = static_cast<float>(JMath::maxInt());
    knowledge.swarm_threshold = (max_int / knowledge.hm_maximum_weight) * 0.00025f;
}


void DirectorBrain::decisionPoint()
{
    updateWorkingKnowledge();

    std::cout << "----------------------------------------------------------\n";
    printDecisionPointLog();
    std::cout << "----------------------------------------------------------\n";
    makeDecision();
    std::cout << "----------------------------------------------------------\n";
}


void DirectorBrain::updateWorkingKnowledge()
{
    int laser_weight = heatmap_manager.getTotalWeight(HeatmapFlag::LASER_DEATHS);
    knowledge.hm_laser_intensity = heatmapWeightToPercentage(laser_weight);

    int bullet_weight = heatmap_manager.getTotalWeight(HeatmapFlag::BULLET_DEATHS);
    knowledge.hm_bullet_intensity = heatmapWeightToPercentage(bullet_weight);

    knowledge.avg_path_diff = 0;
    knowledge.cheapest_path = JMath::maxInt();

    knowledge.total_enemies = enemy_manager.getNumAlive();
    knowledge.fast_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getFastestType());
    knowledge.strong_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getStrongestType());
    knowledge.basic_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getBasicType());
    knowledge.enemies_queued = false;

    for (auto& spawn : enemy_spawns)
    {
        if (!knowledge.enemies_queued && spawn->enemiesQueued())
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
        << "[Brain Decision Point: AT " << JTime::getTime() << "s]\n"
        << "\n"
        << "--HEATMAP INFO:\n"
        << "+  Laser Heatmap Intensity: " << knowledge.hm_laser_intensity  << "/100\n"
        << "+ Bullet Heatmap Intensity: " << knowledge.hm_bullet_intensity << "/100\n"
        << "\n"
        << "--ENEMY INFO:\n"
        << "+            Total Enemies: " << knowledge.total_enemies       << "\n"
        << "+             Fast Enemies: " << knowledge.fast_enemies        << "\n"
        << "+           Strong Enemies: " << knowledge.strong_enemies      << "\n"
        << "+            Basic Enemies: " << knowledge.basic_enemies       << "\n"
        << "+           Enemies Queued: " << enemies_queued_output         << "\n"
        << "\n"
        << "--PATH INFO:\n"
        << "+       Cheapest Path Cost: " << knowledge.cheapest_path       << "\n"
        << "+  Average Path Difference: " << knowledge.avg_path_diff       << "\n";
}


void DirectorBrain::makeDecision()
{
    std::cout << "Decision: ";

    if (knowledge.energy >= 30)
    {
        if (knowledge.hm_bullet_intensity >= knowledge.swarm_threshold)
        {
            auto spawn = enemy_spawns[rand() % enemy_spawns.size()].get();
            auto enemy_type = enemy_manager.getFastestType();

            float spawn_delay = 2 - (enemy_type->speed * 0.02f);
            spawn_delay = JMath::clampf(spawn_delay, 0.5f, 2);

            for (int i = 0; i < 3; ++i)
            {
                spawn->queueEnemy(enemy_type, i * spawn_delay);
            }

            knowledge.energy -= 35;
            std::cout << "Sending Fast Enemy Swarm";
        }
        else
        {
            std::cout << "No Action";
        }
    }
    else
    {
        std::cout << "No Action";
    }

    std::cout << std::endl;
}


float DirectorBrain::heatmapWeightToPercentage(const int _weight)
{
    return (static_cast<float>(_weight) / knowledge.hm_maximum_weight) * 100;
}
