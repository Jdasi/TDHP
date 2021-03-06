#pragma once

#include "Waypoint.h"
#include "Scheduler.h"
#include "LevelPath.h"
#include "TDSprite.h"

namespace sf
{
    class RenderWindow;
}

class Level;
class NavManager;
class EnemyManager;
struct EnemyType;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class which contains information about the location of a spawn position
within a level, and the systems to allow for enemy spawning.

An EnemySpawn automatically refreshes its navigation path
to the enemy destination at a regular interval.

---------------------------------------------------------*/
class EnemySpawn
{
public:
    enum SpawnPathType
    {
        INFLUENCED, // Considers heatmap data.
        PURE        // Ignores heatmap data.
    };

    EnemySpawn(NavManager& _nav_manager, Level& _level, const int _tile_index,
        Waypoint& _enemy_destination, EnemyManager& _enemy_manager);

    ~EnemySpawn() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void setMarkerTexture(sf::Texture* _texture);
    int getTileIndex() const;

    bool enemiesInQueue() const;
    void clearSpawnQueue();

    void spawnEnemy(EnemyType* _type, const SpawnPathType& _path_type = INFLUENCED) const;
    void queueEnemy(EnemyType* _type, const float _delay, const SpawnPathType& _path_type  = INFLUENCED);
    void queueEnemy(EnemyType* _type, const float _delay, const LevelPath& _path);

    int getPathCost() const;
    int getPathDifference() const;
    LevelPath getPath() const;

    void updatePaths();

private:
    void updatePurePath();
    void updateEnemyPath();

    NavManager& nav_manager;
    Level& level;

    Waypoint level_position;
    Waypoint& enemy_destination;
    EnemyManager& enemy_manager;

    LevelPath level_path;
    LevelPath level_path_pure;
    int path_difference;

    Scheduler scheduler;
    TDSprite spawn_marker;

    int enemies_queued;

};
