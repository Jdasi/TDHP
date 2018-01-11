#include <iostream>

#include "DirectorBrain.h"
#include "HeatmapManager.h"
#include "EnemySpawn.h"
#include "EnemyManager.h"
#include "JTime.h"
#include "JMath.h"


DirectorBrain::DirectorBrain(HeatmapManager& _heatmap_manager,
    EnemyManager& _enemy_manager, std::vector<EnemySpawn>& _enemy_spawns)
    : heatmap_manager(_heatmap_manager)
    , enemy_manager(_enemy_manager)
    , enemy_spawns(_enemy_spawns)
{
    scheduler.invokeRepeating([this]()
    {
        decisionPoint();
    }, 5.0f, 5.0f);
}


void DirectorBrain::tick()
{
    scheduler.update();
}


void DirectorBrain::decisionPoint()
{
    updateWorkingKnowledge();
    printDecisionPointLog();
}


void DirectorBrain::updateWorkingKnowledge()
{
    knowledge.hm_total_weight = heatmap_manager.getTotalWeight();
    knowledge.hm_laser_weight = heatmap_manager.getTotalWeight(HeatmapFlag::LASER_DEATHS);
    knowledge.hm_bullet_weight = heatmap_manager.getTotalWeight(HeatmapFlag::BULLET_DEATHS);

    knowledge.avg_path_diff = 0;
    knowledge.cheapest_path = JMath::maxInt();

    knowledge.total_enemies = enemy_manager.getNumAlive();
    knowledge.fast_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getFastestType());
    knowledge.strong_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getStrongestType());
    knowledge.basic_enemies = enemy_manager.getNumAliveOfType(enemy_manager.getBasicType());

    for (auto& spawn : enemy_spawns)
    {
        knowledge.avg_path_diff += spawn.getPathDifference();

        int path_cost = spawn.getPathCost();
        if (path_cost > knowledge.cheapest_path)
            continue;

        knowledge.cheapest_path = path_cost;
    }

    knowledge.avg_path_diff /= enemy_spawns.size();
}


void DirectorBrain::printDecisionPointLog()
{
    std::cout
        << "----------------------------------------------------------\n"
        << "[Brain Decision Point: AT " << JTime::getTime() << "s]\n"
        << "\n"
        << "--HEATMAP INFO:\n"
        << "+  Total Heatmap Intensity: " << knowledge.hm_total_weight  << "\n"
        << "+  Laser Heatmap Intensity: " << knowledge.hm_laser_weight  << "\n"
        << "+ Bullet Heatmap Intensity: " << knowledge.hm_bullet_weight << "\n"
        << "\n"
        << "--ENEMY INFO:\n"
        << "+            Total Enemies: " << knowledge.total_enemies    << "\n"
        << "+             Fast Enemies: " << knowledge.fast_enemies     << "\n"
        << "+           Strong Enemies: " << knowledge.strong_enemies   << "\n"
        << "+            Basic Enemies: " << knowledge.basic_enemies    << "\n"
        << "\n"
        << "--PATH INFO:\n"
        << "+       Cheapest Path Cost: " << knowledge.cheapest_path    << "\n"
        << "+  Average Path Difference: " << knowledge.avg_path_diff    << "\n"
        << "----------------------------------------------------------\n";
}
