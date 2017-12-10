#pragma once

#include <SFML/System/Vector2.hpp>

struct Waypoint
{
    Waypoint()
        : tile_index(0)
        , tile_coords(0, 0)
        , pos(0, 0)
    {
    }

    Waypoint(const int _tile_index, const sf::Vector2i& _coords, const sf::Vector2f& _pos)
        : tile_index(_tile_index)
        , tile_coords(_coords)
        , pos(_pos)
    {
    }

    int tile_index;
    sf::Vector2i tile_coords;
    sf::Vector2f pos;
};
