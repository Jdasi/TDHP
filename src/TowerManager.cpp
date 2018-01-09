#include <iostream>

#include <SFML/Window/Mouse.hpp>

#include "TowerManager.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "EnemyDirector.h"
#include "JHelper.h"
#include "Level.h"
#include "FileIO.h"


TowerManager::TowerManager(AssetManager& _asset_manager, NavManager& _nav_manager,
    EnemyDirector& _enemy_director, Level& _current_level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , enemy_director(_enemy_director)
    , level(_current_level)
    , projectile_manager(_asset_manager, _enemy_director)
    , enemy_destination(enemy_director.getEnemyDestination())
{
    initTowers();

    scheduler.invokeRepeating([this]()
    {
        updateTowerTargets();
    }, 0, 0.05f);
}


void TowerManager::tick(GameData& _gd)
{
    projectile_manager.tick(_gd);
    scheduler.update();

    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        tower.tick(_gd);
    }
}


void TowerManager::draw(sf::RenderWindow& _window)
{
    projectile_manager.draw(_window);

    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        tower.draw(_window);
    }
}


void TowerManager::removeTowerAtPos(const sf::Vector2f& _pos)
{
    int index = JHelper::posToTileIndex(_pos, level);
    if (!JHelper::validIndex(index, level.getProduct()))
        return;

    if (towerExists(index) && nav_manager.isNodeWalkable(index))
    {
        deconstructTower(index);
    }
}


void TowerManager::toggleTowerAtPos(const sf::Vector2f& _pos, int _click_type)
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
            constructTower(index, _pos, clickTypeToTowerSlug(_click_type));
        }
    }
}


void TowerManager::initTowers()
{
    tower_types = FileIO::loadTowerTypes(asset_manager);

    for (auto& tower : towers)
    {
        //tower.attachListener(this);
        tower.init(projectile_manager, enemy_destination);
    }
}


void TowerManager::updateTowerTargets()
{
    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        auto& tower_pos = tower.getPosition();
        auto& nearby_enemies = enemy_director.getEnemiesNearPosSqr(
            tower_pos, tower.getEngageRadiusSqr());

        tower.updateNearbyEnemies(nearby_enemies);
    }
}


void TowerManager::constructTower(const int _tile_index, const sf::Vector2f& _pos,
    const std::string& _tower_slug)
{
    for (auto& tower : towers)
    {
        if (tower.isAlive())
            continue;

        tower.setTileIndex(_tile_index);
        tower.setPosition(_pos);
        tower.setType(tower_types.at(_tower_slug));

        tower.spawn();

        return;
    }
}


void TowerManager::deconstructTower(const int _tile_index)
{
    for (auto& tower : towers)
    {
        if (tower.getTileIndex() != _tile_index)
            continue;

        tower.killQuiet();
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


std::string TowerManager::clickTypeToTowerSlug(const int _click_type)
{
    switch (_click_type)
    {
        case sf::Mouse::Left: return LASER_TOWER_SLUG;
        case sf::Mouse::Right: return BULLET_TOWER_SLUG;

        default: return LASER_TOWER_SLUG;
    }
}
