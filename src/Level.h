#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "LevelData.h"
#include "TileGrid.h"

class Level
{
public:
    enum LevelTileType
    {
        INVALID,
        UNDEFINED,
        WALKABLE,
        UNWALKABLE,
        ENEMY_SPAWN,
        DESTINATION,
        PLAYER_BASE
    };

    Level(const std::string& _level_name);
    ~Level() = default;

    void draw(sf::RenderWindow& _window);

    int getSizeX() const;
    int getSizeY() const;
    int getProduct() const;

    float getTileWidth() const;
    float getTileHeight() const;

    TileGrid& getGrid();
    LevelTileType getLevelTileType(const int _index);

private:
    LevelData level_data;
    TileGrid grid;

};
