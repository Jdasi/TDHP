#pragma once

#include <vector>

#include "LevelData.h"
#include "TileGrid.h"
#include "Waypoint.h"

namespace sf
{
    class RenderWindow;
    class Texture;
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

    LevelTileType getTileType(const int _index) const;
    void updateTileType(const int _index, const LevelTileType& _type);

    void setUnwalkableTexture(sf::Texture* _texture);

    Waypoint createWaypoint(const int _tile_index);

private:
    void init();

    LevelData level_data;
    TileGrid grid;

};
