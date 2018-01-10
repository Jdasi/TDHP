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
    , enemy_type_manager(_asset_manager)
    , brain(_heatmap_manager, enemy_type_manager, enemy_spawns)
{
    enemy_spawns.reserve(MAX_ENEMY_SPAWNS);

    initEnemies();
    initDestinationMarker();

    // Debug repeating enemy spawn.
    scheduler.invokeRepeating([this]()
    {
        if (enemy_spawns.size() > 0)
        {
            EnemyType* random_type = enemy_type_manager.getRandomType();
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
        EnemyType* random_type = enemy_type_manager.getRandomType();
        enemy_spawns[rand() % enemy_spawns.size()].spawnEnemy(random_type);
    }

    for (auto& spawn : enemy_spawns)
    {
        spawn.tick();
    }

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
            continue;

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
    spawn.setMarkerTexture(texture);
}


Waypoint& EnemyDirector::getEnemyDestination()
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


std::vector<Enemy*> EnemyDirector::getEnemiesNearPosSqr(const sf::Vector2f& _pos,
    const float _radius_sqr)
{
    std::vector<Enemy*> nearby_enemies;
    nearby_enemies.reserve(MAX_ENEMIES / 2);

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        float dist = JMath::vector2DistanceSqr(enemy.getPosition(), _pos);
        if (dist > _radius_sqr)
            continue;

        nearby_enemies.push_back(&enemy);
    }

    return nearby_enemies;
}


bool EnemyDirector::damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type)
{
    Enemy* enemy = getEnemyAtPos(_pos);

    if (enemy == nullptr)
        return false;

    enemy->damage(_attacker_type);

    return true;
}


bool EnemyDirector::killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type)
{
    Enemy* enemy = getEnemyAtPos(_pos);

    if (enemy == nullptr)
        return false;

    enemy->kill(_killer_type);

    return true;
}


void EnemyDirector::initEnemies()
{
    for (auto& enemy : enemies)
    {
        enemy.attachListener(this);
    }
}


void EnemyDirector::initDestinationMarker()
{
    auto* texture = asset_manager.loadTexture(DESTINATION_SPRITE);
    destination_marker.setTexture(texture);
}


Enemy* EnemyDirector::getEnemyAtPos(const sf::Vector2f& _pos)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive() || !enemy.collisionCheck(_pos))
            continue;

        return &enemy;
    }

    return nullptr;
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
