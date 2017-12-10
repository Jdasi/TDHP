#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

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
