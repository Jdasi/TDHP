#pragma once

#include <SFML/Graphics.hpp>

struct Waypoint
{
    Waypoint()
        : tile_index(0)
        , pos(0, 0)
    {
    }

    Waypoint(const int _tile_index, const sf::Vector2f& _pos)
        : tile_index(_tile_index)
        , pos(_pos)
    {
    }

    int tile_index;
    sf::Vector2f pos;
};
