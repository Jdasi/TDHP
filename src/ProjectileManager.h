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

class ProjectileManager : public IProjectileManager
{
public:
    ProjectileManager(AssetManager& _asset_manager, HeatmapManager& _heatmap_manager,
        EnemyDirector& _enemy_director, Level& _level);
    ~ProjectileManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void requestProjectile(const ProjectileRequest& _request);

private:
    void initLasers();
    void initBullets(AssetManager& _asset_manager);

    void spawnLaser(const ProjectileRequest& _request);
    void spawnBullet(const ProjectileRequest& _request);

    sf::Vector2f calculateSmokeInaccuracy(const ProjectileRequest& _request);

    HeatmapManager& heatmap_manager;
    EnemyDirector& enemy_director;
    Level& level;

    std::array<TowerLaser, MAX_TOWERS> lasers;
    std::array<TowerBullet, MAX_BULLETS> bullets;
    

};
