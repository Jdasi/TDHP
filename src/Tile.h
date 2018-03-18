#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class which represents a single 2D grid tile.

---------------------------------------------------------*/
class Tile : public sf::RectangleShape
{
public:
    Tile();
    ~Tile() = default;

    const sf::Vector2i& getCoords() const;
    void setCoords(const sf::Vector2i& _coords);

private:
    sf::Vector2i grid_coords;

};
