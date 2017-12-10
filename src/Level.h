#pragma once

#include <vector>

#include "LevelData.h"
#include "TileGrid.h"

namespace sf
{
    class RenderWindow;
}

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
        ENEMY_DESTINATION
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
