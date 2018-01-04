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
#include "FileIO.h"


EnemyDirector::EnemyDirector(AssetManager& _asset_manager, NavManager& _nav_manager,
    HeatmapManager& _heatmap_manager, Level& _level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , heatmap_manager(_heatmap_manager)
    , level(_level)
{
    enemy_spawns.reserve(MAX_ENEMY_SPAWNS);

    initEnemies();
    initDestinationMarker();

    // Debug repeating enemy spawn.
    scheduler.invokeRepeating([this]()
    {
        if (enemy_spawns.size() > 0)
        {
            enemy_spawns[rand() % enemy_spawns.size()].spawnEnemy();
        }
    }, 1, 1);
}


void EnemyDirector::tick(GameData& _gd)
{
    scheduler.update();

    for (auto& spawn : enemy_spawns)
    {
        spawn.tick(_gd);
    }

    // Debug enemy spawn on user input.
    if (_gd.input.getKeyDown(sf::Keyboard::Key::V))
        enemy_spawns[rand() % enemy_spawns.size()].spawnEnemy();

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        enemy.tick();
    }
}


void EnemyDirector::draw(sf::RenderWindow& _window)
{
    for (auto& spawn : enemy_spawns)
    {
        spawn.draw(_window);
    }

    destination_marker.draw(_window);

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
        {
            continue;
        }

        enemy.draw(_window);
    }
}


void EnemyDirector::addEnemySpawn(const int _tile_index)
{
    if (enemy_spawns.size() >= MAX_ENEMY_SPAWNS ||
        !nav_manager.isNodeWalkable(_tile_index))
    {
        return;
    }

    enemy_spawns.emplace_back(nav_manager, level, _tile_index, enemy_destination, enemies);
    auto& spawn = enemy_spawns[enemy_spawns.size() - 1];

    auto* texture = asset_manager.loadTexture(SPAWN_SPRITE);
    spawn.setTexture(texture);
}


const Waypoint& EnemyDirector::getEnemyDestination() const
{
    return enemy_destination;
}


void EnemyDirector::setEnemyDestination(const int _tile_index)
{
    if (!nav_manager.isNodeWalkable(_tile_index))
        return;

    enemy_destination = level.createWaypoint(_tile_index);
    destination_marker.setPosition(enemy_destination.pos);
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


bool EnemyDirector::killEnemyAtPos(const sf::Vector2f& _pos)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive() || !enemy.collisionCheck(_pos))
            continue;

        enemy.kill();
        return true;
    }

    return false;
}


void EnemyDirector::initEnemies()
{
    enemy_types = FileIO::loadEnemyTypes(asset_manager);

    for (auto& enemy : enemies)
    {
        enemy.attachListener(this);

        // Debug random selection.
        std::array<std::string, 3> slugs { LIGHT_ENEMY_SLUG, MEDIUM_ENEMY_SLUG, HEAVY_ENEMY_SLUG };
        std::string selection = slugs[rand() % slugs.size()];

        enemy.setType(enemy_types.at(selection));
    }
}


void EnemyDirector::initDestinationMarker()
{
    auto* texture = asset_manager.loadTexture(DESTINATION_SPRITE);
    destination_marker.setTexture(texture);
}


// EnemyListener Event: Called when an enemy is killed.
void EnemyDirector::onDeath(const sf::Vector2f& _pos)
{
    int tile_index = JHelper::posToTileIndex(_pos, level);
    heatmap_manager.splashOnHeatmap(0, tile_index, 3);
}


// EnemyListener Event: Called when an enemy finishes traversing its path.
void EnemyDirector::onPathComplete(Enemy& _caller)
{
    _caller.killQuiet();
}
