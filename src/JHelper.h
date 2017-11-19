#pragma once

#include <algorithm>

#include <SFML/Graphics.hpp>

struct Level;

/* Various functions that don't belong in one specific place and are useful
 * in many areas.
 */
namespace JHelper
{
    int calculateIndex(const unsigned int _x, const unsigned int _y, const unsigned int _size_x);
    int calculateIndex(const sf::Vector2i& _pos, const unsigned int _size_x);
    sf::Vector2i calculateCoords(const unsigned int _index, const unsigned int _size_x);
    bool validIndex(const int _index, const int _array_size);

    bool posInSimulationArea(const sf::Vector2f& _pos);
    int posToTileIndex(const sf::Vector2f& _pos, const Level& _level);

    template <typename T>
    void centerSFOrigin(T& _sfml_object)
    {
        auto bounds = _sfml_object.getLocalBounds();
        _sfml_object.setOrigin(bounds.left + bounds.width / 2.0f,
            bounds.top + bounds.height / 2.0f);
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
