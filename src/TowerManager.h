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
class HeatmapManager;
class Level;
struct Waypoint;
struct GameData;

class TowerManager
{
public:
    TowerManager(AssetManager& _asset_manager, NavManager& _nav_manager,
        HeatmapManager& _heatmap_manager, EnemyDirector& _enemy_director,
        Level& _level);
    ~TowerManager() = default;

    void tick(GameData& _gd);
    void draw(sf::RenderWindow& _window);

    void removeTower(const int _tile_index);
    void toggleTower(const int _tile_index, const int _click_type);

private:
    void initTowers();
    void updateTowerTargets();

    void constructTower(const int _tile_index, const std::string& _tower_slug);
    void deconstructTower(const int _tile_index);

    bool towerExists(const int _tile_index) const;
    std::string clickTypeToTowerSlug(const int _click_type);

    AssetManager& asset_manager;
    NavManager& nav_manager;
    HeatmapManager& heatmap_manager;
    EnemyDirector& enemy_director;
    Level& level;

    ProjectileManager projectile_manager;
    std::map<std::string, TowerType> tower_types;
    std::array<Tower, MAX_TOWERS> towers;

    Waypoint& enemy_destination;
    Scheduler scheduler;

};
