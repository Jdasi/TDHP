#pragma once

#include <array>
#include <map>

#include <SFML/System/Vector2.hpp>

#include "Tower.h"
#include "Constants.h"
#include "Scheduler.h"
#include "ProjectileManager.h"

namespace sf
{
    class RenderWindow;
}

class AssetManager;
class NavManager;
class EnemyDirector;
class Level;
struct Waypoint;

class TowerManager
{
public:
    TowerManager(AssetManager& _asset_manager, NavManager& _nav_manager,
        EnemyDirector& _enemy_director, Level& _current_level);

    ~TowerManager() = default;

    void tick(GameData& _gd);
    void draw(sf::RenderWindow& _window);

    void removeTowerAtPos(const sf::Vector2f& _pos);
    void toggleTowerAtPos(const sf::Vector2f& _pos, const int _click_type);

private:
    void initTowers();
    void updateTowerTargets();

    void constructTower(const int _tile_index, const sf::Vector2f& _pos,
        const std::string& _tower_slug);
    void deconstructTower(const int _tile_index);

    bool towerExists(const int _tile_index) const;
    std::string clickTypeToTowerSlug(const int _click_type);

    AssetManager& asset_manager;
    NavManager& nav_manager;
    EnemyDirector& enemy_director;
    Level& level;

    ProjectileManager projectile_manager;
    std::map<std::string, TowerType> tower_types;
    std::array<Tower, MAX_TOWERS> towers;

    Waypoint& enemy_destination;
    Scheduler scheduler;

};
