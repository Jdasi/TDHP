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
class EnemyDirector;

class ProjectileManager : public IProjectileManager
{
public:
    ProjectileManager(AssetManager& _asset_manager, EnemyDirector& _enemy_director);
    ~ProjectileManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void requestProjectile(const ProjectileRequest& _request);

private:
    void initLasers();
    void initBullets(AssetManager& _asset_manager);

    void spawnLaser(const ProjectileRequest& _request);
    void spawnBullet(const ProjectileRequest& _request);

    EnemyDirector& enemy_director;

    std::array<TowerLaser, MAX_TOWERS> lasers;
    std::array<TowerBullet, MAX_BULLETS> bullets;
    

};
