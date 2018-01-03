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
}


void EnemySpawn::draw(sf::RenderWindow& _window)
{
    spawn_marker.draw(_window);
}


void EnemySpawn::setTexture(sf::Texture* _texture)
{
    spawn_marker.setTexture(_texture);
}


/* Finds the first dead enemy in the pool and respawns them at the passed position.
 */
void EnemySpawn::spawnEnemy()
{
    for (auto& enemy : enemies)
    {
        if (enemy.isAlive())
            continue;

        enemy.spawn();
        enemy.setPosition(level_position.pos);

        updateEnemyPath();
        enemy.setPath(level_path);

        return;
    }
}


void EnemySpawn::updateEnemyPath()
{
    auto path = nav_manager.findPath(level_position.tile_coords, enemy_destination.tile_coords);
    level_path = LevelPath(level, path);
}
