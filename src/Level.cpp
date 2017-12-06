#include "Level.h"
#include "FileIO.h"
#include "Constants.h"
#include "JHelper.h"


Level::Level(const std::string& _level_name)
    : level_data(FileIO::loadLevelData(_level_name))
    , grid(*this, WALKABLE_COLOR)
{
}


void Level::draw(sf::RenderWindow& _window)
{
    grid.draw(_window);
}


int Level::getSizeX() const
{
    return level_data.size_x;
}


int Level::getSizeY() const
{
    return level_data.size_y;
}


int Level::getProduct() const
{
    return level_data.product;
}


float Level::getTileWidth() const
{
    return level_data.tile_width;
}


float Level::getTileHeight() const
{
    return level_data.tile_height;
}


TileGrid& Level::getGrid()
{
    return grid;
}


Level::LevelTileType Level::getLevelTileType(const int _index)
{
    LevelTileType type = LevelTileType::INVALID;

    if (!JHelper::validIndex(_index, getProduct()))
        return type;

    switch (level_data.raw_data[_index])
    {
        case '.': return LevelTileType::WALKABLE;
        case 'W': return LevelTileType::UNWALKABLE;

        default: return LevelTileType::UNDEFINED;
    }
}
