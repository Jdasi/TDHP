#include "Level.h"
#include "FileIO.h"
#include "Constants.h"
#include "JHelper.h"


Level::Level(const std::string& _level_name)
    : level_data(FileIO::loadLevelData(_level_name))
    , grid(*this, WALKABLE_COLOR)
{
    init();
}


void Level::draw(sf::RenderWindow& _window)
{
    grid.draw(_window);
}


const std::string& Level::getName() const
{
    return level_data.name;
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


Level::LevelTileType Level::getTileType(const int _index) const
{
    LevelTileType type = LevelTileType::INVALID;

    if (!JHelper::validIndex(_index, getProduct()))
        return type;

    return static_cast<LevelTileType>(level_data.tile_data[_index]);
}


void Level::updateTileType(const int _index, const LevelTileType& _type)
{
    if (!JHelper::validIndex(_index, getProduct()))
        return;

    level_data.tile_data[_index] = _type;

    switch (_type)
    {
        case WALKABLE: { grid.setTileColor(_index, WALKABLE_COLOR); } break;
        case UNWALKABLE: { grid.setTileColor(_index, UNWALKABLE_COLOR); } break;
        case ENEMY_SPAWN: { grid.setTileColor(_index, WALKABLE_COLOR); } break;

        default: {}
    }
}


void Level::setUnwalkableTexture(sf::Texture* _texture)
{
    grid.setTileTexture(_texture);
}


Waypoint Level::createWaypoint(const int _tile_index)
{
    auto coords = JHelper::calculateCoords(_tile_index, getSizeX());
    auto pos = JHelper::tileIndexToTileCenter(_tile_index, *this);

    return Waypoint(_tile_index, coords, pos);
}


void Level::init()
{
    for (int i = 0; i < level_data.product; ++i)
    {
        updateTileType(i, getTileType(i));
    }
}
