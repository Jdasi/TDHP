#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "EnemyDirector.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "InputHandler.h"
#include "GameData.h"
#include "HeatmapManager.h"
#include "JHelper.h"
#include "JMath.h"
#include "Level.h"


EnemyDirector::EnemyDirector(AssetManager& _asset_manager, NavManager& _nav_manager,
    HeatmapManager& _heatmap_manager, Level& _level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , heatmap_manager(_heatmap_manager)
    , current_level(_level)
{
    initEnemies();
    initDestinationMarker();
}


void EnemyDirector::tick(GameData& _gd)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        enemy.tick();
    }

    // DEBUG.
    if (_gd.input.getKeyDown(sf::Keyboard::Key::V))
    {
        if (enemy_spawns.size() > 0)
        {
            // TODO: pick a random spawn ..
            spawnEnemy(enemy_spawns[0].pos);
        }
    }
}


void EnemyDirector::draw(sf::RenderWindow& _window)
{
    for (auto& marker : spawn_markers)
    {
        _window.draw(marker);
    }

    _window.draw(destination_marker);
    level_path.draw(_window);

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
        {
            continue;
        }

        _window.draw(enemy);
    }
}


void EnemyDirector::addEnemySpawn(const int _tile_index)
{
    if (!nav_manager.isNodeWalkable(_tile_index))
        return;

    enemy_spawns.push_back(createWaypoint(_tile_index));
    auto& waypoint = enemy_spawns[enemy_spawns.size() - 1];

    auto* texture = asset_manager.loadTexture("spawn.png");
    auto texture_size = texture->getSize();

    sf::Sprite marker(*texture);
    marker.setPosition(waypoint.pos);
    marker.setScale(current_level.getTileWidth() / texture_size.x,
        current_level.getTileHeight() / texture_size.y);

    JHelper::centerSFOrigin(marker);
    spawn_markers.push_back(marker);
}


const Waypoint& EnemyDirector::getEnemyDestination() const
{
    return enemy_destination;
}


void EnemyDirector::setEnemyDestination(const int _tile_index)
{
    if (!nav_manager.isNodeWalkable(_tile_index))
        return;

    enemy_destination = createWaypoint(_tile_index);
    destination_marker.setPosition(enemy_destination.pos);

    // DEBUG.
    auto& spawn = enemy_spawns[0];
    auto path = nav_manager.findPath(spawn.tile_coords, enemy_destination.tile_coords);
    level_path = LevelPath(current_level, path);
}


std::vector<Enemy*> EnemyDirector::getEnemiesNearPosSqr(const sf::Vector2f& _pos, const float _radius_sqr)
{
    std::vector<Enemy*> nearby_enemies;
    nearby_enemies.reserve(MAX_ENEMIES / 2);

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
        {
            continue;
        }

        float dist = JMath::vector2DistanceSqr(enemy.getPosition(), _pos);
        if (dist > _radius_sqr)
        {
            continue;
        }

        nearby_enemies.push_back(&enemy);
    }

    return nearby_enemies;
}


void EnemyDirector::initEnemies()
{
    auto* texture = asset_manager.loadTexture(ENEMY_SPRITE);
    auto texture_size = texture->getSize();

    for (auto& enemy : enemies)
    {
        enemy.attachListener(this);

        enemy.setTexture(*texture);
        enemy.setScale(current_level.getTileWidth() / texture_size.x,
            current_level.getTileHeight() / texture_size.y);

        JHelper::centerSFOrigin(enemy);
    }
}


void EnemyDirector::initDestinationMarker()
{
    auto* texture = asset_manager.loadTexture(DESTINATION_SPRITE);
    auto texture_size = texture->getSize();

    destination_marker.setTexture(*texture);
    destination_marker.setScale(current_level.getTileWidth() / texture_size.x,
        current_level.getTileHeight() / texture_size.y);

    JHelper::centerSFOrigin(destination_marker);
}


Waypoint EnemyDirector::createWaypoint(const int _tile_index)
{
    auto coords = JHelper::calculateCoords(_tile_index, current_level.getSizeX());
    auto pos = JHelper::tileIndexToPos(_tile_index, current_level);

    return Waypoint(_tile_index, coords, pos);
}


/* Finds the first dead enemy in the pool and respawns them at the passed position.
 */
void EnemyDirector::spawnEnemy(const sf::Vector2f& _pos)
{
    for (auto& enemy : enemies)
    {
        if (enemy.isAlive())
            continue;

        enemy.spawn();
        enemy.setPosition(_pos);

        return;
    }
}


// EnemyListener Event: Called when an enemy is killed.
void EnemyDirector::onDeath(const sf::Vector2f& _pos)
{
    int tile_index = JHelper::posToTileIndex(_pos, current_level);
    heatmap_manager.splashOnHeatmap(0, tile_index, 3);
}
