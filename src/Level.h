#pragma once

#include <vector>

#include "LevelData.h"
#include "TileGrid.h"
#include "Waypoint.h"

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

    LevelTileType getLevelTileType(const int _index);

    void setTileColor(const int _index, const sf::Color& _color);

    Waypoint createWaypoint(const int _tile_index);

private:
    LevelData level_data;
    TileGrid grid;

};
