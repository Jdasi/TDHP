#pragma once

#include <SFML/System/Vector2.hpp>

#include "Enemy.h"
#include "EnemyListener.h"
#include "EnemyType.h"
#include "EnemyManager.h"
#include "EnemySpawn.h"
#include "Constants.h"
#include "LevelPath.h"
#include "Scheduler.h"
#include "DirectorBrain.h"

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

    Waypoint& getEnemyDestination();
    void setEnemyDestination(const int _tile_index);

    std::vector<Enemy*> getEnemiesNearPosSqr(const sf::Vector2f& _pos,
        const float _radius_sqr);

    bool damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type);
    bool killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type = nullptr);

private:
    void initDestinationMarker();

    // Enemy events.
    void onDeath(const sf::Vector2f& _pos, TowerType* _killer_type) override;
    void onPathComplete(Enemy& _enemy) override;

    AssetManager& asset_manager;
    NavManager& nav_manager;
    HeatmapManager& heatmap_manager;
    Level& level;

    EnemyManager enemy_manager;
    std::vector<EnemySpawn> enemy_spawns;

    Waypoint enemy_destination;
    TDSprite destination_marker;

    Scheduler scheduler;
    DirectorBrain brain;

};
