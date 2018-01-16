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

    switch (level_data.raw_data[_index])
    {
        case '.': return LevelTileType::WALKABLE;
        case 'W': return LevelTileType::UNWALKABLE;
        case 'S': return LevelTileType::ENEMY_SPAWN;
        case 'D': return LevelTileType::ENEMY_DESTINATION;

        default: return LevelTileType::UNDEFINED;
    }
}


void Level::updateTileType(const int _index, const LevelTileType& _type)
{
    if (!JHelper::validIndex(_index, getProduct()))
        return;

    auto& c = level_data.raw_data[_index];

    switch (_type)
    {
        case WALKABLE:
        {
            c = '.';
            grid.setTileColor(_index, WALKABLE_COLOR);
        } break;

        case UNWALKABLE:
        {
            c = 'W';
            grid.setTileColor(_index, UNWALKABLE_COLOR);
        } break;

        case ENEMY_SPAWN:
        {
            c = 'S';
        } break;

        case ENEMY_DESTINATION:
        {
            c = 'D';
        } break;

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
