#pragma once

#include <array>
#include <vector>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>

#include "Enemy.h"
#include "Waypoint.h"
#include "EnemyListener.h"
#include "Constants.h"
#include "LevelPath.h"

namespace sf
{
    class RenderWindow;
}

class AssetManager;
class NavManager;
class HeatmapManager;
class Level;
struct GameData;

class EnemyDirector : public EnemyListener
{
public:
    EnemyDirector(AssetManager& _asset_manager, NavManager& _nav_manager,
        HeatmapManager& _heatmap_manager, Level& _level);
    ~EnemyDirector() = default;

    void tick(GameData& _gd);
    void draw(sf::RenderWindow& _window);

    void addEnemySpawn(const int _tile_index);

    const Waypoint& getEnemyDestination() const;
    void setEnemyDestination(const int _tile_index);

    std::vector<Enemy*> getEnemiesNearPosSqr(const sf::Vector2f& _pos,
        const float _radius_sqr);

private:
    void initEnemies();
    void initDestinationMarker();

    Waypoint createWaypoint(const int _tile_index);
    void spawnEnemy(const sf::Vector2f& _pos);

    // Enemy events.
    void onDeath(const sf::Vector2f& _pos) override;

    AssetManager& asset_manager;
    NavManager& nav_manager;
    HeatmapManager& heatmap_manager;
    Level& current_level;

    std::array<Enemy, MAX_ENEMIES> enemies;

    std::vector<Waypoint> enemy_spawns;
    std::vector<sf::Sprite> spawn_markers;

    Waypoint enemy_destination;
    sf::Sprite destination_marker;

    LevelPath level_path;

};
