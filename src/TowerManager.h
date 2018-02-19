#pragma once

#include <array>
#include <map>

#include <SFML/Graphics/Text.hpp>

#include "Tower.h"
#include "Constants.h"
#include "Scheduler.h"
#include "ProjectileManager.h"

namespace sf
{
    class RenderWindow;
}

class NavManager;
class EnemyDirector;
class HeatmapManager;
class Level;
struct Waypoint;
struct GameData;

class TowerManager
{
public:
    TowerManager(GameData& _game_data, NavManager& _nav_manager,
        HeatmapManager& _heatmap_manager, EnemyDirector& _enemy_director,
        Level& _level);
    ~TowerManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void removeTower(const int _tile_index);
    void toggleTower(const int _tile_index, const int _click_type);

private:
    void initTowers();
    void initTowerSlots();
    void updateTowerTargets();

    void constructTower(const int _tile_index, const std::string& _tower_slug);
    void deconstructTower(const int _tile_index);

    bool towerExists(const int _tile_index) const;
    std::string clickTypeToTowerSlug(const int _click_type);

    void increaseSlotsUsed();
    void decreaseSlotsUsed();

    GameData& gd;

    NavManager& nav_manager;
    HeatmapManager& heatmap_manager;
    EnemyDirector& enemy_director;
    Level& level;

    ProjectileManager projectile_manager;
    std::map<std::string, TowerType> tower_types;
    std::array<Tower, MAX_TOWERS> towers;
    std::array<sf::Sprite, MAX_TOWERS> tower_slots;

    Waypoint& enemy_destination;
    Scheduler scheduler;

    sf::Text lbl_slots;
    int slots_used;

};
