#pragma once

#include <array>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Enemy.h"
#include "Constants.h"
#include "EnemyListener.h"

class AssetManager;
class NavManager;
class HeatmapManager;
struct GameData;
struct Level;

class EnemyDirector : public EnemyListener
{
public:
    EnemyDirector(AssetManager& _asset_manager, NavManager& _nav_manager,
        HeatmapManager& _heatmap_manager, Level& _level);
    ~EnemyDirector() = default;

    void tick(GameData& _gd);
    void draw(sf::RenderWindow& _window);

    std::vector<Enemy*> getEnemiesNearPosSqr(const sf::Vector2f& _pos,
        const float _radius_sqr);

private:
    void initEnemies();
    void spawnEnemy(const sf::Vector2f& _pos);

    // Enemy events.
    void onDeath(const sf::Vector2f& _pos) override;

    AssetManager& asset_manager;
    NavManager& nav_manager;
    HeatmapManager& heatmap_manager;
    Level& current_level;

    sf::Texture* enemy_texture;
    std::array<Enemy, MAX_ENEMIES> enemies;

};
