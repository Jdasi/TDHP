#pragma once

#include <algorithm>
#include <string>

#include <SFML/System/Vector2.hpp>

class Level;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Functions that don't belong in one specific place and are useful
for many aspects of the program.

---------------------------------------------------------*/
namespace JHelper
{
    int calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x);
    int calculateIndex(const sf::Vector2i& _coords, const unsigned int _size_x);
    sf::Vector2i calculateCoords(const unsigned int _index, const unsigned int _size_x);
    bool validIndex(const int _index, const int _array_size);

    bool posInSimulationArea(const sf::Vector2f& _pos);
    int posToTileIndex(const sf::Vector2f& _pos, const float _tile_width,
        const float _tile_height, const int _level_size_x);
    int posToTileIndex(const sf::Vector2f& _pos, const Level& _level);

    sf::Vector2f tileIndexToTileCenter(const int _tile_index, const float _tile_width,
        const float _tile_height, const int _level_size_x);
    sf::Vector2f tileIndexToTileCenter(const int _tile_index, const Level& _level);

    int manhattanDistance(const sf::Vector2i& _a, const sf::Vector2i& _b);
    int chebyshevDistance(const sf::Vector2i& _a, const sf::Vector2i& _b);

    float calculateLookAngle(const sf::Vector2f& _from, const sf::Vector2f& _to);

    std::string boolToStr(const bool _b);
    std::string timeToStr(const double _time);


    template <typename T>
    void centerSFOrigin(T& _sfml_object)
    {
        auto bounds = _sfml_object.getLocalBounds();
        _sfml_object.setOrigin(bounds.left + bounds.width * 0.5f,
            bounds.top + bounds.height * 0.5f);
    }


    template <typename T, typename U>
    auto findInVectorPair(T& _vector, U& _key)
    {
        auto entry = std::find_if(_vector.begin(), _vector.end(),
            [_key](const auto& _elem)
        {
            return _elem.first == _key;
        });

        return entry;
    }


    template <typename T>
    void sortVectorPair(T& _vector)
    {
        std::sort(_vector.begin(), _vector.end(),
            [](const auto& _lhs, const auto& _rhs)
        {
            return _lhs.first < _rhs.first;
        });
    }

}
