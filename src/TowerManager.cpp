#include <iostream>

#include "TowerManager.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "EnemyDirector.h"
#include "JHelper.h"
#include "JMath.h"
#include "Level.h"


TowerManager::TowerManager(AssetManager& _asset_manager, NavManager& _nav_manager,
    EnemyDirector& _enemy_director, Level& _current_level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , enemy_director(_enemy_director)
    , level(_current_level)
{
    initTowers();
}


void TowerManager::tick()
{
    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        auto& tower_pos = tower.getPosition();
        auto& nearby_enemies = enemy_director.getEnemiesNearPosSqr(
            tower_pos, TOWER_ENGAGE_RADIUS_SQR);

        // Nothing to shoot.
        if (nearby_enemies.size() == 0)
            continue;

        Enemy* closest_enemy = evaluateClosestEnemy(nearby_enemies,
            tower_pos);
        tower.engage(closest_enemy);
    }
}


void TowerManager::draw(sf::RenderWindow& _window)
{
    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        tower.draw(_window);
    }
}


void TowerManager::toggleTowerAtPos(const sf::Vector2f& _pos)
{
    int index = JHelper::posToTileIndex(_pos, level);
    if (!JHelper::validIndex(index, level.getProduct()))
        return;

    if (towerExists(index))
    {
        deconstructTower(index);
    }
    else
    {
        if (!nav_manager.isNodeWalkable(index))
        {
            constructTower(index, _pos);
        }
    }
}


void TowerManager::initTowers()
{
    auto* tower_texture = asset_manager.loadTexture(TOWER_SPRITE);

    for (auto& tower : towers)
    {
        //tower.attachListener(this);
        tower.setTexture(tower_texture);
    }
}


void TowerManager::constructTower(const int _tile_index, const sf::Vector2f& _pos)
{
    for (auto& tower : towers)
    {
        if (tower.isAlive())
            continue;

        tower.spawn();
        tower.setTileIndex(_tile_index);
        tower.setPosition(_pos);

        return;
    }
}


void TowerManager::deconstructTower(const int _tile_index)
{
    for (auto& tower : towers)
    {
        if (tower.getTileIndex() != _tile_index)
            continue;

        tower.kill();
        tower.setTileIndex(-1);

        return;
    }
}


bool TowerManager::towerExists(const int _tile_index) const
{
    for (auto& tower : towers)
    {
        if (tower.getTileIndex() == _tile_index)
            return true;
    }

    return false;
}


// TODO: return an enemy interface, rather than the whole enemy ..
Enemy* TowerManager::evaluateClosestEnemy(const std::vector<Enemy*>& _enemies,
    const sf::Vector2f& _pos)
{
    Enemy* closest_enemy = nullptr;
    float closest_dist = JMath::maxFloat();

    for (auto* enemy : _enemies)
    {
        float dist = JMath::vector2DistanceSqr(enemy->getPosition(), _pos);
        if (dist > closest_dist)
        {
            continue;
        }

        closest_enemy = enemy;
        closest_dist = dist;
    }

    return closest_enemy;
}
