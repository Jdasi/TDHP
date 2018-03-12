#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "EnemySpawn.h"
#include "Level.h"
#include "NavManager.h"
#include "EnemyManager.h"
#include "GDebugFlags.h"


EnemySpawn::EnemySpawn(NavManager& _nav_manager, Level& _level, 
    const int _tile_index, Waypoint& _enemy_destination, EnemyManager& _enemy_manager)
    : nav_manager(_nav_manager)
    , level(_level)
    , level_position(level.createWaypoint(_tile_index))
    , enemy_destination(_enemy_destination)
    , enemy_manager(_enemy_manager)
    , enemies_queued(0)
{
    spawn_marker.setPosition(level_position.pos);

    // Regularly update path influenced by heatmap data.
    scheduler.invokeRepeating([this]()
    {
        if (!level_path_pure.pathSuccessful())
            updatePurePath();

        updateEnemyPath();
    }, 0, 0.5f);
}


void EnemySpawn::tick()
{
    scheduler.update();
}


void EnemySpawn::draw(sf::RenderWindow& _window)
{
    spawn_marker.draw(_window);

    if (GDebugFlags::draw_paths)
        level_path_pure.draw(_window, level_position.pos, 0, sf::Color::Red);
}


void EnemySpawn::setMarkerTexture(sf::Texture* _texture)
{
    spawn_marker.setTexture(_texture);
}


int EnemySpawn::getTileIndex() const
{
    return level_position.tile_index;
}


bool EnemySpawn::enemiesInQueue() const
{
    return enemies_queued > 0;
}


void EnemySpawn::clearSpawnQueue()
{
    scheduler.cancelInvoke("QueueEnemy");
    enemies_queued = 0;
}


/* Finds the first dead enemy in the pool and respawns them at the passed position.
 * The spawned enemy assumes the characteristics of the passed type.
 */
void EnemySpawn::spawnEnemy(EnemyType* _type, const SpawnPathType& _path_type) const
{
    if (!level_path.pathSuccessful() || _type == nullptr)
        return;

    const LevelPath& path = _path_type == INFLUENCED ? level_path : level_path_pure;
    enemy_manager.spawnEnemy(_type, level_position.pos, path);
}


void EnemySpawn::queueEnemy(EnemyType* _type, const float _delay,
    const SpawnPathType& _path_type)
{
    const LevelPath& path = _path_type == INFLUENCED ? level_path : level_path_pure;
    queueEnemy(_type, _delay, path);
}


void EnemySpawn::queueEnemy(EnemyType* _type, const float _delay, const LevelPath& _path)
{
    scheduler.invoke([this, _type, _path]()
    {
        enemy_manager.spawnEnemy(_type, level_position.pos, _path);

        --enemies_queued;
    }, _delay, "QueueEnemy");

    ++enemies_queued;
}


int EnemySpawn::getPathCost() const
{
    return level_path.getTotalCost();
}


int EnemySpawn::getPathDifference() const
{
    return path_difference;
}


LevelPath EnemySpawn::getPath() const
{
    return level_path;
}


void EnemySpawn::updatePaths()
{
    updatePurePath();
    updateEnemyPath();
}


// Calculate straight path to destination, ignoring heatmap data.
void EnemySpawn::updatePurePath()
{
    auto path = nav_manager.findPath(
        level_position.tile_coords,
        enemy_destination.tile_coords,
        HeatmapFlag::NONE);

    level_path_pure = LevelPath(level, path);
}


// Update path influenced by heatmap data.
void EnemySpawn::updateEnemyPath()
{
    auto path = nav_manager.findPath(
        level_position.tile_coords,
        enemy_destination.tile_coords,
        HeatmapFlag::DIRECTOR);

    level_path = LevelPath(level, path);
    path_difference = level_path.getTotalCost() - level_path_pure.getTotalCost();
}
