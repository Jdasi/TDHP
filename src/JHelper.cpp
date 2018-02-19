#include "JHelper.h"
#include "JMath.h"
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
    int x = _index % _size_x;
    int y = _index / _size_x;

    return sf::Vector2i(x, y);
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
    const float _tile_height, const int _level_size_x)
{
    float offsetx = _pos.x - WINDOW_LEFT_BOUNDARY;
    int ix = static_cast<int>(offsetx / _tile_width);

    float offsety = _pos.y - WINDOW_TOP_BOUNDARY;
    int iy = static_cast<int>(offsety / _tile_height);

    return JHelper::calculateIndex(ix, iy, _level_size_x);
}


int JHelper::posToTileIndex(const sf::Vector2f& _pos, const Level& _level)
{
    return posToTileIndex(_pos, _level.getTileWidth(), _level.getTileHeight(), _level.getSizeX());
}


sf::Vector2f JHelper::tileIndexToTileCenter(const int _tile_index, const float _tile_width,
    const float _tile_height, const int _level_width)
{
    auto coords = calculateCoords(_tile_index, _level_width);

    float half_width = _tile_width / 2;
    float half_height = _tile_height / 2;

    float x = WINDOW_LEFT_BOUNDARY + static_cast<float>(coords.x * _tile_width) + half_width;
    float y = WINDOW_TOP_BOUNDARY + static_cast<float>(coords.y * _tile_height) + half_height;

    return sf::Vector2f(x, y);
}


sf::Vector2f JHelper::tileIndexToTileCenter(const int _tile_index, const Level& _level)
{
    return tileIndexToTileCenter(_tile_index, _level.getTileWidth(), _level.getTileHeight(), _level.getSizeX());
}


int JHelper::manhattanDistance(const sf::Vector2i& _a, const sf::Vector2i& _b)
{
    return abs(_a.x - _b.x) + abs(_a.y - _b.y);
}


int JHelper::chebyshevDistance(const sf::Vector2i& _a, const sf::Vector2i& _b)
{
    return std::max(abs(_a.x - _b.x), abs(_a.y - _b.y));
}


float JHelper::calculateLookAngle(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    float dx = _to.x - _from.x;
    float dy = _to.y - _from.y;

    return ((atan2(dy, dx)) * 180 / JMath::PI) + 90;
}


std::string JHelper::boolToStr(const bool _b)
{
    return _b ? "TRUE" : "FALSE";
}


std::string JHelper::timeToString(const float _time)
{
    int t = static_cast<int>(_time);

    int hours = (t / 60) / 60;
    int minutes = (t / 60) - (hours * 60);
    int seconds = t % 60;

    std::string time_str =
        std::to_string(hours / 10) + std::to_string(hours % 10) + ":" +
        std::to_string(minutes / 10) + std::to_string(minutes % 10) + ":" +
        std::to_string(seconds / 10) + std::to_string(seconds % 10);

    return time_str;
}
