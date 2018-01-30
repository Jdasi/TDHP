#include <iostream>
#include <iomanip>

#include "DirectorBrain.h"
#include "HeatmapManager.h"
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


void DirectorBrain::grantEnergy(const float _amount)
{
    knowledge.energy += _amount;
    knowledge.energy = JMath::clampf(knowledge.energy, 0, MAX_DIRECTOR_ENERGY);
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
    int overall_weight = heatmap_manager.getTotalWeight();
    knowledge.hm_overall_intensity = heatmapWeightToPercentage(overall_weight);

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
        << "[Brain Decision Point at " << JTime::getTime() << "s with " << knowledge.energy << " energy]\n"
        << "\n"
        << "--HEATMAP INFO:\n"
        << "+ Overall Heatmap Intensity: " << knowledge.hm_overall_intensity << "/100\n"
        << "+   Laser Heatmap Intensity: " << knowledge.hm_laser_intensity   << "/100\n"
        << "+  Bullet Heatmap Intensity: " << knowledge.hm_bullet_intensity  << "/100\n"
        << "\n"
        << "--ENEMY INFO:\n"
        << "+             Total Enemies: " << knowledge.total_enemies        << "\n"
        << "+              Fast Enemies: " << knowledge.fast_enemies         << "\n"
        << "+            Strong Enemies: " << knowledge.strong_enemies       << "\n"
        << "+             Basic Enemies: " << knowledge.basic_enemies        << "\n"
        << "+            Enemies Queued: " << enemies_queued_output          << "\n"
        << "\n"
        << "--PATH INFO:\n"
        << "+        Cheapest Path Cost: " << knowledge.cheapest_path        << "\n"
        << "+   Average Path Difference: " << knowledge.avg_path_diff        << "\n";
}


void DirectorBrain::makeDecision()
{
    std::cout << "Decision: ";

    if (knowledge.energy == MAX_DIRECTOR_ENERGY)
    {
        processEnergyTierOne();
    }
    else if (knowledge.energy >= 75)
    {
        processEnergyTierTwo();
    }
    else if (knowledge.energy >= 50)
    {
        processEnergyTierThree();
    }
    else if (knowledge.energy >= 25)
    {
        processEnergyTierFour();
    }
    else
    {
        noAction();
    }

    std::cout << std::endl;
}


float DirectorBrain::heatmapWeightToPercentage(const int _weight)
{
    return (static_cast<float>(_weight) / knowledge.hm_maximum_weight) * 100;
}


// Performs an action choosing from those available to energy level 100.
void DirectorBrain::processEnergyTierOne()
{
    if (tierOneActions())
        return;

    if (tierTwoActions())
        return;

    if (tierThreeActions())
        return;

    if (tierFourActions())
        return;

    noAction();
}


// Performs an action choosing from those available to energy level 99 or lower.
void DirectorBrain::processEnergyTierTwo()
{
    if (tierTwoActions())
        return;

    if (tierThreeActions())
        return;

    if (tierFourActions())
        return;

    noAction();
}


// Performs an action choosing from those available to energy level 74 or lower.
void DirectorBrain::processEnergyTierThree()
{
    if (tierThreeActions())
        return;

    if (tierFourActions())
        return;

    noAction();
}


// Performs an action choosing from those available to energy level 49 or lower.
void DirectorBrain::processEnergyTierFour()
{
    if (tierFourActions())
        return;

    noAction();
}


bool DirectorBrain::laserIntensityOverThreshold() const
{
    return knowledge.hm_laser_intensity >= knowledge.swarm_threshold;
}


bool DirectorBrain::bulletIntensityOverThreshold() const
{
    return knowledge.hm_bullet_intensity >= knowledge.swarm_threshold;
}


bool DirectorBrain::overallIntensityOverThreshold() const
{
    return knowledge.hm_overall_intensity >= knowledge.swarm_threshold * 2;
}


bool DirectorBrain::fastEnemiesOverThreshold() const
{
    return static_cast<float>(knowledge.fast_enemies) / MAX_ENEMIES >= 0.05f;
}


bool DirectorBrain::strongEnemiesOverThreshold() const
{
    return static_cast<float>(knowledge.strong_enemies) / MAX_ENEMIES >= 0.05f;
}


bool DirectorBrain::totalEnemiesOverThreshold() const
{
    return static_cast<float>(knowledge.total_enemies) / MAX_ENEMIES >= 0.1f;
}


/* Actions specific to energy level 100.
 * Returns true if an action was performed, otherwise returns false.
 */
bool DirectorBrain::tierOneActions()
{
    if (totalEnemiesOverThreshold())
    {
        speedBoostAllEnemies();
        healthBoostAllEnemies();

        knowledge.energy -= 100;
    }
    else
    {
        sendBasicSwarm();
        knowledge.energy -= 50;

        return true;
    }

    return false;
}


/* Actions specific to energy range 75-99.
 * Returns true if an action was performed, otherwise returns false.
 */
bool DirectorBrain::tierTwoActions()
{
    if (totalEnemiesOverThreshold())
    {
        if (knowledge.hm_bullet_intensity > knowledge.hm_laser_intensity)
        {
            speedBoostAllEnemies();
        }
        else
        {
            healthBoostAllEnemies();
        }

        knowledge.energy -= 75;

        return true;
    }

    return false;
}


/* Actions specific to energy range 50-74.
 * Returns true if an action was performed, otherwise returns false.
 */
bool DirectorBrain::tierThreeActions()
{
    if (laserIntensityOverThreshold())
    {
        sendStrongSwarm();
        knowledge.energy -= 50;

        return true;
    }
    else if (bulletIntensityOverThreshold())
    {
        sendFastSwarm();
        knowledge.energy -= 50;

        return true;
    }
    else if (overallIntensityOverThreshold())
    {
        sendBasicSwarm();
        knowledge.energy -= 50;

        return true;
    }
    
    return false;
}


/* Actions specific to energy range 0-49.
 * Returns true if an action was performed, otherwise returns false.
 */
bool DirectorBrain::tierFourActions()
{
    if (fastEnemiesOverThreshold())
    {
        if (laserIntensityOverThreshold())
        {
            healthBoostFastEnemies();
        }
        else
        {
            speedBoostFastEnemies();
        }

        knowledge.energy -= 30;

        return true;
    }
    else if (strongEnemiesOverThreshold())
    {
        if (bulletIntensityOverThreshold())
        {
            speedBoostStrongEnemies();
        }
        else
        {
            healthBoostStrongEnemies();
        }

        knowledge.energy -= 30;

        return true;
    }

    return false;
}


void DirectorBrain::noAction() const
{
    std::cout << "No Action";
}


void DirectorBrain::sendFastSwarm()
{
    sendSwarm(enemy_manager.getFastestType(), 3, EnemySpawn::SpawnPathType::INFLUENCED);

    std::cout << "Sending Fast Enemy Swarm";
}


void DirectorBrain::sendStrongSwarm()
{
    sendSwarm(enemy_manager.getStrongestType(), 3, EnemySpawn::SpawnPathType::PURE);

    std::cout << "Sending Strong Enemy Swarm";
}


void DirectorBrain::sendBasicSwarm()
{
    sendSwarm(enemy_manager.getBasicType(), 3, EnemySpawn::SpawnPathType::INFLUENCED);

    std::cout << "Sending Basic Enemy Swarm";
}


void DirectorBrain::healthBoostFastEnemies()
{
    auto type = enemy_manager.getFastestType();
    enemy_manager.boostEnemyHealth(type, 2, 5);

    std::cout << "Boosting Fast Enemy Health";
}


void DirectorBrain::healthBoostStrongEnemies()
{
    auto type = enemy_manager.getStrongestType();
    enemy_manager.boostEnemyHealth(type, 2, 5);

    std::cout << "Boosting Strong Enemy Health";
}


void DirectorBrain::healthBoostAllEnemies()
{
    enemy_manager.boostEnemyHealth(2, 5);

    std::cout << "Boosting All Enemy Health";
}


void DirectorBrain::speedBoostFastEnemies()
{
    auto type = enemy_manager.getFastestType();
    enemy_manager.boostEnemySpeed(type, 2, 3);

    std::cout << "Boosting Fast Enemy Speed";
}


void DirectorBrain::speedBoostStrongEnemies()
{
    auto type = enemy_manager.getStrongestType();
    enemy_manager.boostEnemySpeed(type, 2, 3);

    std::cout << "Boosting Strong Enemy Speed";
}


void DirectorBrain::speedBoostAllEnemies()
{
    enemy_manager.boostEnemySpeed(2, 3);

    std::cout << "Boosting All Enemy Speed";
}


void DirectorBrain::sendSwarm(EnemyType* _type, const int _count, const EnemySpawn::SpawnPathType& _path)
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
        best_spawn->queueEnemy(_type, i * spawn_delay, _path);
    }
}
