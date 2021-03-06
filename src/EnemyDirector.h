#pragma once

#include <memory>

#include <SFML/Graphics/Text.hpp>

#include "Enemy.h"
#include "EnemyListener.h"
#include "EnemyType.h"
#include "EnemyManager.h"
#include "EnemySpawn.h"
#include "LevelPath.h"
#include "Scheduler.h"
#include "DirectorBrain.h"
#include "ValueBar.h"

namespace sf
{
    class RenderWindow;
}

class NavManager;
class HeatmapManager;
class Level;
struct GameData;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Encapsulates all the data and logic related to the adversarial
AI force which is trying to reach the player base as part of
the Tower Defence game.

---------------------------------------------------------*/
class EnemyDirector final
{
public:
    EnemyDirector(GameData& _game_data, NavManager& _nav_manager,
        HeatmapManager& _heatmap_manager, Level& _level);

    ~EnemyDirector() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    bool spawnExists(const int _tile_index);
    bool addEnemySpawn(const int _tile_index);
    bool removeEnemySpawn(const int _tile_index);
    void toggleEnemySpawn(const int _tile_index);
    void updatePaths();

    void addEnemyListener(EnemyListener* _listener);

    Waypoint& getEnemyDestination();
    void setEnemyDestination(const int _tile_index);

    std::vector<Enemy*> getEnemiesNearPosSqr(const sf::Vector2f& _pos,
        const float _radius_sqr);

    bool damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type);
    bool killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type = nullptr);

private:
    void init();
    void initVisualisations();
    void initRegularSpawning();

    void handleDebugCommands();

    GameData& gd;

    NavManager& nav_manager;
    HeatmapManager& heatmap_manager;
    Level& level;

    EnemyManager enemy_manager;
    std::vector<std::unique_ptr<EnemySpawn>> enemy_spawns;

    Waypoint enemy_destination;
    TDSprite destination_marker;

    Scheduler scheduler;
    DirectorBrain brain;
    ValueBar energy_bar;

    sf::Text lbl_energy;

};
