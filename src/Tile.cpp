#include "Tile.h"


Tile::Tile()
    : grid_coords()
{
}


const sf::Vector2i& Tile::getCoords() const
{
    return grid_coords;
}


void Tile::setCoords(const sf::Vector2i& _coords)
{
    grid_coords = _coords;
}
