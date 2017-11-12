#include "JHelper.h"


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
