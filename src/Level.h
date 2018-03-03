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
        INVALID = -2,
        UNDEFINED = -1,
        WALKABLE,
        UNWALKABLE,
        ENEMY_SPAWN,
        ENEMY_DESTINATION
    };

    Level(LevelData* _ld);
    ~Level() = default;

    void draw(sf::RenderWindow& _window);

    const std::string& getName() const;
    const std::string& getDescription() const;

    int getSizeX() const;
    int getSizeY() const;
    int getProduct() const;

    int getHighestScore() const;
    double getLongestSurvivedTime() const;

    float getTileWidth() const;
    float getTileHeight() const;

    LevelTileType getTileType(const int _index) const;
    void updateTileType(const int _index, const LevelTileType& _type);

    void setUnwalkableTexture(sf::Texture* _texture);

    Waypoint createWaypoint(const int _tile_index);

private:
    void init();

    LevelData* ld;
    TileGrid grid;

};
