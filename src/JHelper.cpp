#include "JHelper.h"
#include "Constants.h"
#include "Level.h"


int JHelper::calculateIndex(const unsigned _x, const unsigned _y, const unsigned _size_x)
{
    return (_y * _size_x) + _x;
}


int JHelper::calculateIndex(const sf::Vector2i & _pos, const unsigned int _size_x)
{
    return calculateIndex(_pos.x, _pos.y, _size_x);
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


int JHelper::posToTileIndex(const sf::Vector2f& _pos, const Level& _level)
{
    float offsetx = _pos.x - WINDOW_MARGIN_X;
    int ix = static_cast<int>(offsetx / _level.tile_width);

    float offsety = _pos.y - WINDOW_MARGIN_Y;
    int iy = static_cast<int>(offsety / _level.tile_height);

    return JHelper::calculateIndex(ix, iy, _level.width);
}
