#include <iostream>

#include "DirectorBrain.h"
#include "HeatmapManager.h"
#include "EnemySpawn.h"
#include "JTime.h"
#include "JMath.h"


DirectorBrain::DirectorBrain(HeatmapManager& _heatmap_manager,
    EnemyTypeManager& _enemy_type_manager, std::vector<EnemySpawn>& _enemy_spawns)
    : heatmap_manager(_heatmap_manager)
    , enemy_type_manager(_enemy_type_manager)
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
        << "+  Total Heatmap Intensity: " << knowledge.hm_total_weight << "\n"
        << "+  Laser Heatmap Intensity: " << knowledge.hm_laser_weight << "\n"
        << "+ Bullet Heatmap Intensity: " << knowledge.hm_bullet_weight << "\n"
        << "\n"
        << "--ENEMY INFO:\n"
        << "+       Total Fast Enemies: " << 0 << "\n"
        << "+     Total Normal Enemies: " << 0 << "\n"
        << "+     Total Strong Enemies: " << 0 << "\n"
        << "\n"
        << "--PATH INFO:\n"
        << "+       Cheapest Path Cost: " << knowledge.cheapest_path << "\n"
        << "+  Average Path Difference: " << knowledge.avg_path_diff << "\n"
        << "----------------------------------------------------------\n";
}
