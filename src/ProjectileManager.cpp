#include "ProjectileManager.h"
#include "AssetManager.h"
#include "EnemyDirector.h"
#include "JTime.h"
#include "JMath.h"
#include "TowerType.h"


ProjectileManager::ProjectileManager(AssetManager& _asset_manager, EnemyDirector& _enemy_director)
    : enemy_director(_enemy_director)
{
    initLasers();
    initBullets(_asset_manager);
}


void ProjectileManager::tick(GameData& _gd)
{
    for (auto& bullet : bullets)
    {
        if (!bullet.isActive())
            continue;

        bullet.tick();

        if (enemy_director.killEnemyAtPos(bullet.getPosition()))
        {
            bullet.destroy();
            break;
        }
    }
}


void ProjectileManager::draw(sf::RenderWindow& _window)
{
    for (auto& laser : lasers)
    {
        if (!laser.isActive())
            continue;

        laser.draw(_window);
    }

    for (auto& bullet : bullets)
    {
        if (!bullet.isActive())
            continue;

        bullet.draw(_window);
    }
}


void ProjectileManager::requestProjectile(const ProjectileRequest& _request)
{
    if (_request.tower_type == nullptr || _request.tower_target == nullptr)
        return;

    auto& tower_slug = _request.tower_type->slug;

    if (tower_slug == "LaserTower")
    {
        spawnLaser(_request);
    }
    else if (tower_slug == "BulletTower")
    {
        spawnBullet(_request);
    }
}


void ProjectileManager::initLasers()
{
    for (auto& laser : lasers)
    {
        laser.setActiveDuration(0.1f);
        laser.setColor(sf::Color::Cyan);
    }
}


void ProjectileManager::initBullets(AssetManager& _asset_manager)
{
    auto* texture = _asset_manager.loadTexture(BULLET_SPRITE);

    for (auto& bullet : bullets)
    {
        bullet.setActiveDuration(3);
        bullet.setTexture(texture);
        bullet.setColor(sf::Color::Yellow);
    }
}


void ProjectileManager::spawnLaser(const ProjectileRequest& _request)
{
    for (auto& laser : lasers)
    {
        if (laser.isActive())
            continue;

        laser.refresh(_request.tower_pos, _request.tower_target->getPosition());
        laser.setStats(_request.tower_type->projectile_stats);

        break;
    }

    _request.tower_target->kill();
}


void ProjectileManager::spawnBullet(const ProjectileRequest& _request)
{
    for (auto& bullet : bullets)
    {
        if (bullet.isActive())
            continue;

        bullet.refresh(_request.tower_pos, _request.tower_target->getPosition());
        bullet.setStats(_request.tower_type->projectile_stats);

        break;
    }
}
