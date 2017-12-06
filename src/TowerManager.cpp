#include <iostream>

#include "TowerManager.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "EnemyDirector.h"
#include "Constants.h"
#include "JHelper.h"
#include "JMath.h"
#include "Level.h"


TowerManager::TowerManager(AssetManager& _asset_manager, NavManager& _nav_manager,
    EnemyDirector& _enemy_director, Level& _current_level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , enemy_director(_enemy_director)
    , current_level(_current_level)
{
    tower_texture = asset_manager.loadTexture("tower.png");
}


void TowerManager::tick()
{
    for (auto& tower : towers)
    {
        if (tower->canShoot())
        {
            auto& tower_pos = tower->getPosition();
            auto& nearby_enemies = enemy_director.getEnemiesNearPosSqr(
                tower_pos, TOWER_ENGAGE_RADIUS_SQR);

            // Nothing to shoot.
            if (nearby_enemies.size() == 0)
            {
                continue;
            }

            Enemy* closest_enemy = evaluateClosestEnemy(nearby_enemies,
                tower_pos);
            tower->shoot(closest_enemy);
        }
    }
}


void TowerManager::draw(sf::RenderWindow& _window)
{
    for (auto& tower : towers)
    {
        tower->draw(_window);
    }
}


void TowerManager::buildTowerAtPos(const sf::Vector2f& _pos)
{
    int index = JHelper::posToTileIndex(_pos, current_level);
    if (!JHelper::validIndex(index, current_level.getProduct()))
        return;

    if (!tileEligibleForBuild(index))
        return;

    auto tower = std::make_unique<Tower>(index);
    tower->setTexture(tower_texture);
    tower->setPosition(_pos);
    
    auto texture_size = tower_texture->getSize();
    tower->setScale(current_level.getTileWidth() / texture_size.x,
        current_level.getTileHeight() / texture_size.y);

    towers.push_back(std::move(tower));
}


bool TowerManager::tileEligibleForBuild(const int _tile_index) const
{
    if (nav_manager.isTileWalkable(_tile_index))
        return false;

    for (auto& tower : towers)
    {
        if (tower->getTileIndex() == _tile_index)
            return false;
    }

    return true;
}


// TODO: return an enemy interface, rather than the whole enemy ..
Enemy* TowerManager::evaluateClosestEnemy(const std::vector<Enemy*>& _enemies,
    const sf::Vector2f& _pos)
{
    Enemy* closest_enemy = nullptr;
    float closest_dist = JMath::maxFloat();

    for (auto* enemy : _enemies)
    {
        float dist = JMath::Vector2DistanceSqr(enemy->getPosition(), _pos);
        if (dist > closest_dist)
        {
            continue;
        }

        closest_enemy = enemy;
        closest_dist = dist;
    }

    return closest_enemy;
}
