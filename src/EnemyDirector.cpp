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
#include "TowerType.h"


EnemyDirector::EnemyDirector(AssetManager& _asset_manager, NavManager& _nav_manager,
    HeatmapManager& _heatmap_manager, Level& _level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , heatmap_manager(_heatmap_manager)
    , level(_level)
    , enemy_manager(_asset_manager)
    , brain(_heatmap_manager, enemy_manager, enemy_spawns)
{
    enemy_spawns.reserve(MAX_ENEMY_SPAWNS);
    enemy_manager.addEnemyListener(this);

    initDestinationMarker();

    // Debug repeating enemy spawn.
    scheduler.invokeRepeating([this]()
    {
        if (enemy_spawns.size() > 0)
        {
            EnemyType* random_type = enemy_manager.getRandomType();
            enemy_spawns[rand() % enemy_spawns.size()].spawnEnemy(random_type);
        }
    }, 2.0f, 2.0f);
}


void EnemyDirector::tick(GameData& _gd)
{
    scheduler.update();
    brain.tick();

    // Debug enemy spawn on user input.
    if (_gd.input.getKeyDown(sf::Keyboard::Key::V))
    {
        EnemyType* random_type = enemy_manager.getRandomType();
        enemy_spawns[rand() % enemy_spawns.size()].spawnEnemy(random_type);
    }

    for (auto& spawn : enemy_spawns)
    {
        spawn.tick();
    }

    enemy_manager.tick();
}


void EnemyDirector::draw(sf::RenderWindow& _window)
{
    for (auto& spawn : enemy_spawns)
    {
        spawn.draw(_window);
    }

    destination_marker.draw(_window);
    enemy_manager.draw(_window);
}


void EnemyDirector::addEnemySpawn(const int _tile_index)
{
    if (enemy_spawns.size() >= MAX_ENEMY_SPAWNS ||
        !nav_manager.isNodeWalkable(_tile_index))
    {
        return;
    }

    enemy_spawns.emplace_back(nav_manager, level, _tile_index, enemy_destination, enemy_manager);
    auto& spawn = enemy_spawns[enemy_spawns.size() - 1];

    auto* texture = asset_manager.loadTexture(SPAWN_SPRITE);
    spawn.setMarkerTexture(texture);

    spawn.updatePurePath();
}


Waypoint& EnemyDirector::getEnemyDestination()
{
    return enemy_destination;
}


void EnemyDirector::setEnemyDestination(const int _tile_index)
{
    if (!JHelper::validIndex(_tile_index, level.getProduct()))
        return;

    enemy_destination = level.createWaypoint(_tile_index);
    destination_marker.setPosition(enemy_destination.pos);

    for (auto& spawn : enemy_spawns)
    {
        spawn.updatePurePath();
    }
}


std::vector<Enemy*> EnemyDirector::getEnemiesNearPosSqr(const sf::Vector2f& _pos,
    const float _radius_sqr)
{
    return enemy_manager.getEnemiesNearPosSqr(_pos, _radius_sqr);
}


bool EnemyDirector::damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type)
{
    return enemy_manager.damageEnemyAtPos(_pos, _attacker_type);
}


bool EnemyDirector::killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type)
{
    return enemy_manager.killEnemyAtPos(_pos, _killer_type);
}


void EnemyDirector::initDestinationMarker()
{
    auto* texture = asset_manager.loadTexture(DESTINATION_SPRITE);
    destination_marker.setTexture(texture);
}


// EnemyListener Event: Called when an enemy is killed.
void EnemyDirector::onDeath(const sf::Vector2f& _pos, TowerType* _killer_type)
{
    int tile_index = JHelper::posToTileIndex(_pos, level);

    if (_killer_type != nullptr)
    {
        if (_killer_type->slug == LASER_TOWER_SLUG)
        {
            heatmap_manager.splashOnHeatmap(HeatmapFlag::LASER_DEATHS, tile_index, 3);
        }
        else if (_killer_type->slug == BULLET_TOWER_SLUG)
        {
            heatmap_manager.splashOnHeatmap(HeatmapFlag::BULLET_DEATHS, tile_index, 3);
        }
    }
}


// EnemyListener Event: Called when an enemy finishes traversing its path.
void EnemyDirector::onPathComplete(Enemy& _caller)
{
    _caller.killQuiet();
}
