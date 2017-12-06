#include <iostream>

#include "EnemyDirector.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "InputHandler.h"
#include "GameData.h"
#include "HeatmapManager.h"
#include "JHelper.h"
#include "JMath.h"
#include "Level.h"


EnemyDirector::EnemyDirector(AssetManager& _asset_manager, NavManager& _nav_manager,
    HeatmapManager& _heatmap_manager, Level& _level)
    : asset_manager(_asset_manager)
    , nav_manager(_nav_manager)
    , heatmap_manager(_heatmap_manager)
    , current_level(_level)
{
    initEnemies();
}


void EnemyDirector::tick(GameData& _gd)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        enemy.tick();
    }

    // DEBUG.
    if (_gd.input.getKeyDown(sf::Keyboard::Key::V))
    {
        spawnEnemy(sf::Vector2f(static_cast<float>(WINDOW_LEFT_BOUNDARY),
            static_cast<float>(WINDOW_HEIGHT) / 2));
    }
}


void EnemyDirector::draw(sf::RenderWindow& _window)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
        {
            continue;
        }

        _window.draw(enemy);
    }
}


std::vector<Enemy*> EnemyDirector::getEnemiesNearPosSqr(const sf::Vector2f& _pos, const float _radius_sqr)
{
    std::vector<Enemy*> nearby_enemies;
    nearby_enemies.reserve(MAX_ENEMIES / 2);

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
        {
            continue;
        }

        float dist = JMath::Vector2DistanceSqr(enemy.getPosition(), _pos);
        if (dist > _radius_sqr)
        {
            continue;
        }

        nearby_enemies.push_back(&enemy);
    }

    return nearby_enemies;
}


void EnemyDirector::initEnemies()
{
    enemy_texture = asset_manager.loadTexture("enemy.png");
    auto texture_size = enemy_texture->getSize();

    for (auto& enemy : enemies)
    {
        enemy.attachListener(this);

        enemy.setTexture(*enemy_texture);
        enemy.setScale(current_level.tile_width / texture_size.x,
            current_level.tile_height / texture_size.y);

        JHelper::centerSFOrigin(enemy);
    }
}


/* Finds the first dead enemy in the pool and respawns them at the passed position.
 */
void EnemyDirector::spawnEnemy(const sf::Vector2f& _pos)
{
    for (auto& enemy : enemies)
    {
        if (enemy.isAlive())
        {
            continue;
        }

        enemy.spawn();
        enemy.setPosition(_pos);

        // We're done here.
        return;
    }
}


// EnemyListener Event: Called when an enemy is killed.
void EnemyDirector::onDeath(const sf::Vector2f& _pos)
{
    int tile_index = JHelper::posToTileIndex(_pos, current_level);
    heatmap_manager.splashOnHeatmap(0, tile_index, 3);
}
