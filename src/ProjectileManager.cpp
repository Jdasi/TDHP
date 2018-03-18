#include "ProjectileManager.h"
#include "ProjectileStats.h"
#include "AssetManager.h"
#include "GameAudio.h"
#include "HeatmapManager.h"
#include "EnemyDirector.h"
#include "Level.h"
#include "JTime.h"
#include "JMath.h"
#include "TowerType.h"
#include "GameData.h"


ProjectileManager::ProjectileManager(GameData& _game_data, HeatmapManager& _heatmap_manager,
    EnemyDirector& _enemy_director, Level& _level)
    : gd(_game_data)
    , heatmap_manager(_heatmap_manager)
    , enemy_director(_enemy_director)
    , level(_level)
{
    initLasers();
    initBullets();
}


void ProjectileManager::tick()
{
    for (auto& bullet : bullets)
    {
        if (!bullet.isActive())
            continue;

        bullet.tick();

        auto& bullet_pos = bullet.getPosition();
        if (!JHelper::posInSimulationArea(bullet_pos))
        {
            bullet.expire();
        }

        if (enemy_director.damageEnemyAtPos(bullet_pos, bullet.getOwningType()))
        {
            gd.audio.playSound(BULLET_HIT_SOUND);
            bullet.expire();
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

    auto& projectile_slug = _request.tower_type->projectile_stats.slug;

    if (projectile_slug == LASER_PROJ_SLUG)
    {
        spawnLaser(_request);
    }
    else if (projectile_slug == BULLET_PROJ_SLUG)
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


void ProjectileManager::initBullets()
{
    auto* texture = gd.assets.loadTexture(BULLET_TEXTURE);

    for (auto& bullet : bullets)
    {
        bullet.setActiveDuration(3);
        bullet.setTexture(texture);
        bullet.setColor(sf::Color::Yellow);
    }
}


void ProjectileManager::spawnLaser(const ProjectileRequest& _request)
{
    gd.audio.playSound(LASER_SHOOT_SOUND);

    sf::Vector2f inaccuracy = calculateSmokeInaccuracy(_request);
    sf::Vector2f target_pos = _request.tower_target->getPosition() + inaccuracy;

    for (auto& laser : lasers)
    {
        if (laser.isActive())
            continue;

        laser.setOwningType(_request.tower_type);
        laser.refresh(_request.tower_pos, target_pos);

        break;
    }

    if (target_pos == _request.tower_target->getPosition())
    {
        // Bypass unnecessary collision checks with accurate shots.
        _request.tower_target->damage(_request.tower_type);
    }
    else
    {
        enemy_director.damageEnemyAtPos(target_pos, _request.tower_type);
    }
}


void ProjectileManager::spawnBullet(const ProjectileRequest& _request)
{
    gd.audio.playSound(BULLET_SHOOT_SOUND);

    sf::Vector2f inaccuracy = calculateSmokeInaccuracy(_request);
    sf::Vector2f target_pos = _request.tower_target->getPosition() + inaccuracy;

    for (auto& bullet : bullets)
    {
        if (bullet.isActive())
            continue;

        bullet.setOwningType(_request.tower_type);
        bullet.refresh(_request.tower_pos, target_pos);

        break;
    }
}


/*
Calculates inaccuracy from smoke based on the position of the request's target.
The inaccuracy is also determined by the level's tile size.
*/
sf::Vector2f ProjectileManager::calculateSmokeInaccuracy(const ProjectileRequest& _request)
{
    int tile_index = _request.tower_target->getTileIndex();
    int smoke_weight = heatmap_manager.getWeight(tile_index, HeatmapFlag::SMOKE, true);

    if (smoke_weight == 0)
        return { 0, 0 };

    float half_width = level.getTileWidth() / 2;
    float half_height = level.getTileHeight() / 2;

    float rand_x = static_cast<float>(rand() % static_cast<int>(level.getTileWidth())) - half_width;
    float rand_y = static_cast<float>(rand() % static_cast<int>(level.getTileHeight())) - half_height;

    rand_x *= smoke_weight / (MAX_WEIGHTING * 0.5f);
    rand_y *= smoke_weight / (MAX_WEIGHTING * 0.5f);

    return { rand_x, rand_y };
}
