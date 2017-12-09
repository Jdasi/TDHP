#include "JHelper.h"
#include "Constants.h"
#include "Level.h"


int JHelper::calculateIndex(const unsigned _x, const unsigned _y, const unsigned _size_x)
{
    return (_y * _size_x) + _x;
}


int JHelper::calculateIndex(const sf::Vector2i & _coords, const unsigned int _size_x)
{
    return calculateIndex(_coords.x, _coords.y, _size_x);
}


sf::Vector2i JHelper::calculateCoords(const unsigned int _index, const unsigned int _size_x)
{
    sf::Vector2i coords;

    coords.x = _index % _size_x;
    coords.y = _index / _size_x;

    return coords;
}


bool JHelper::validIndex(const int _index, const int _array_size)
{
    if (_index < 0 || _index >= _array_size)
        return false;

    return true;
}


bool JHelper::posInSimulationArea(const sf::Vector2f& _pos)
{
    if (_pos.x < WINDOW_LEFT_BOUNDARY ||
        _pos.x >= WINDOW_RIGHT_BOUNDARY ||
        _pos.y < WINDOW_TOP_BOUNDARY ||
        _pos.y >= WINDOW_BOTTOM_BOUNDARY)
    {
        return false;
    }

    return true;
}


int JHelper::posToTileIndex(const sf::Vector2f& _pos, const float _tile_width,
    const float _tile_height, const int _level_width)
{
    float offsetx = _pos.x - WINDOW_MARGIN_X;
    int ix = static_cast<int>(offsetx / _tile_width);

    float offsety = _pos.y - WINDOW_MARGIN_Y;
    int iy = static_cast<int>(offsety / _tile_height);

    return JHelper::calculateIndex(ix, iy, _level_width);
}


int JHelper::posToTileIndex(const sf::Vector2f& _pos, const Level& _level)
{
    return posToTileIndex(_pos, _level.getTileWidth(), _level.getTileHeight(), _level.getSizeX());
}


int JHelper::manhattanDistance(const sf::Vector2i& _a, const sf::Vector2i& _b)
{
    return abs(_a.x - _b.x) + abs(_a.y - _b.y);
}


int JHelper::chebyshevDistance(const sf::Vector2i& _a, const sf::Vector2i& _b)
{
    return std::max(abs(_a.x - _b.x), abs(_a.y - _b.y));
}
