#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Tile.h"

namespace sf
{
    class RenderWindow;
    class Color;
    class Texture;
}

class Level;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class to visualise a 2D grid of square tiles.

---------------------------------------------------------*/
class TileGrid final
{
public:
    TileGrid(const Level& _level, const sf::Color& _initial_color);
    ~TileGrid() = default;

    void draw(sf::RenderWindow& _window);

    int getSizeX() const;
    int getSizeY() const;

    void setTileAlpha(const int _tile_index, const float _alpha);
    void modifyTileAlpha(const int _tile_index, const float _amount);
    void setTileColor(const int _tile_index, const sf::Color& _color);

    void setTileTexture(sf::Texture* _texture);

private:
    void init(const sf::Color& _initial_color);

    int size_x;
    int size_y;

    float tile_width;
    float tile_height;

    std::vector<Tile> tiles;

};
