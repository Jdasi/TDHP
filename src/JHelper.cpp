#include "JHelper.h"


int JHelper::calculateIndex(const unsigned _x, const unsigned _y, const unsigned _size_x)
{
    return (_y * _size_x) + _x;
}


int JHelper::calculateIndex(const sf::Vector2i & _pos, const unsigned int _size_x)
{
    return calculateIndex(_pos.x, _pos.y, _size_x);
}
