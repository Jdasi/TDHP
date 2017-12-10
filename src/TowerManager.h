#pragma once

#include <array>

#include <SFML/System/Vector2.hpp>

#include "Tower.h"
#include "Constants.h"

namespace sf
{
    class RenderWindow;
}

class AssetManager;
class NavManager;
class EnemyDirector;
class Level;

class TowerManager
{
public:
    TowerManager(AssetManager& _asset_manager, NavManager& _nav_manager,
        EnemyDirector& _enemy_director, Level& _current_level);

    ~TowerManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void toggleTowerAtPos(const sf::Vector2f& _pos);

private:
    void initTowers();
    void constructTower(const int _tile_index, const sf::Vector2f& _pos);
    void deconstructTower(const int _tile_index);

    bool towerExists(const int _tile_index) const;
    Enemy* evaluateClosestEnemy(const std::vector<Enemy*>& _enemies,
        const sf::Vector2f& _pos);

    AssetManager& asset_manager;
    NavManager& nav_manager;
    EnemyDirector& enemy_director;
    Level& current_level;

    std::array<Tower, MAX_TOWERS> towers;

};
