#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "EnemySpawn.h"
#include "Level.h"
#include "NavManager.h"


EnemySpawn::EnemySpawn(NavManager& _nav_manager, Level& _level, 
    const int _tile_index, Waypoint& _enemy_destination, std::array<Enemy, MAX_ENEMIES>& _enemies)
    : nav_manager(_nav_manager)
    , level(_level)
    , level_position(level.createWaypoint(_tile_index))
    , enemy_destination(_enemy_destination)
    , enemies(_enemies)
{
    spawn_marker.setPosition(level_position.pos);

    // Regularly update path influenced by heatmap data.
    scheduler.invokeRepeating([this]()
    {
        if (!level_path_pure.pathSuccessful())
            calculatePurePath();

        updateEnemyPath();
    }, 0, 0.5f);
}


void EnemySpawn::tick(GameData& _gd)
{
    scheduler.update();
}


void EnemySpawn::draw(sf::RenderWindow& _window)
{
    spawn_marker.draw(_window);
}


void EnemySpawn::setMarkerTexture(sf::Texture* _texture)
{
    spawn_marker.setTexture(_texture);
}


/* Finds the first dead enemy in the pool and respawns them at the passed position.
 * The spawned enemy assumes the characteristics of the passed type.
 */
void EnemySpawn::spawnEnemy(EnemyType* _type)
{
    if (!level_path.pathSuccessful() || _type == nullptr)
        return;

    for (auto& enemy : enemies)
    {
        if (enemy.isAlive())
            continue;

        enemy.setType(*_type);
        enemy.spawn();

        enemy.setPosition(level_position.pos);
        enemy.setPath(level_path);

        return;
    }
}


int EnemySpawn::getPathDifference()
{
    return path_difference;
}


// Calculate straight path to destination, ignoring heatmap data.
void EnemySpawn::calculatePurePath()
{
    auto path = nav_manager.findPath(
        level_position.tile_coords,
        enemy_destination.tile_coords);

    level_path_pure = LevelPath(level, path);
}


// Update path influenced by heatmap data.
void EnemySpawn::updateEnemyPath()
{
    auto path = nav_manager.findPath(
        level_position.tile_coords,
        enemy_destination.tile_coords,
        HeatmapFlag::TURRETS);

    level_path = LevelPath(level, path);
    path_difference = level_path.getTotalCost() - level_path_pure.getTotalCost();
}
