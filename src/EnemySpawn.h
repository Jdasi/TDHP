#pragma once

#include <array>

#include <SFML/Graphics/Sprite.hpp>

#include "Constants.h"
#include "Waypoint.h"
#include "Enemy.h"

namespace sf
{
    class RenderWindow;
}

class Level;
class NavManager;

class EnemySpawn
{
public:
    EnemySpawn(NavManager& _nav_manager, Level& _level, const int _tile_index,
        Waypoint& _enemy_destination, std::array<Enemy, MAX_ENEMIES>& _enemies);

    ~EnemySpawn() = default;

    void draw(sf::RenderWindow& _window);

    void setTexture(sf::Texture* _texture);

    void spawnEnemy();

private:
    void updateEnemyPath();

    NavManager& nav_manager;
    Level& level;

    Waypoint level_position;
    Waypoint& enemy_destination;
    std::array<Enemy, MAX_ENEMIES>& enemies;

    LevelPath level_path;

    TDSprite spawn_marker;

};
