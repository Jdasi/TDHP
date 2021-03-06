#pragma once

#include <array>

#include "IProjectileManager.h"
#include "TDSprite.h"
#include "Constants.h"
#include "TowerLaser.h"
#include "TowerBullet.h"
#include "ProjectileRequest.h"

namespace sf
{
    class RenderWindow;
}

class AssetManager;
class HeatmapManager;
class EnemyDirector;
class Level;
struct GameData;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class concerned with the Projectiles that the TowerManager and its
member systems can use to shoot at enemy units.

Derives the IProjectileManager interface to expose limited
usage information to Towers.

---------------------------------------------------------*/
class ProjectileManager final : public IProjectileManager
{
public:
    ProjectileManager(GameData& _game_data, HeatmapManager& _heatmap_manager,
        EnemyDirector& _enemy_director, Level& _level);

    ~ProjectileManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void requestProjectile(const ProjectileRequest& _request);

private:
    void initLasers();
    void initBullets();

    void spawnLaser(const ProjectileRequest& _request);
    void spawnBullet(const ProjectileRequest& _request);

    sf::Vector2f calculateSmokeInaccuracy(const ProjectileRequest& _request);

    GameData& gd;

    HeatmapManager& heatmap_manager;
    EnemyDirector& enemy_director;
    Level& level;

    std::array<TowerLaser, MAX_TOWERS> lasers;
    std::array<TowerBullet, MAX_BULLETS> bullets;
    

};
