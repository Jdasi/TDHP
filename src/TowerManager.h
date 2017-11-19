#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Tower.h"

class AssetManager;
class NavManager;
class EnemyDirector;
struct GameData;
struct Level;

class TowerManager
{
public:
    TowerManager(AssetManager* _asset_manager, NavManager* _nav_manager,
        EnemyDirector* _enemy_director, Level* _current_level);

    ~TowerManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void buildTowerOnTile(const int _tile_index);

private:
    bool tileEligibleForBuild(const int _tile_index) const;
    Enemy* evaluateClosestEnemy(const std::vector<Enemy*>& _enemies,
        const sf::Vector2f& _pos);

    AssetManager* asset_manager;
    NavManager* nav_manager;
    EnemyDirector* enemy_director;
    Level* current_level;

    sf::Texture* tower_texture;
    std::vector<std::unique_ptr<Tower>> towers;

};
