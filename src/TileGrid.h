#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Tile.h"

namespace sf
{
    class RenderWindow;
    class Color;
}

class Level;

class TileGrid
{
public:
    TileGrid(const Level& _level, const sf::Color& _initial_color);
    ~TileGrid() = default;

    int getSizeX() const;
    int getSizeY() const;

    int posToTileIndex(const sf::Vector2f& _pos);
    sf::Vector2f tileIndexToPos(const int _tile_index);

    void setTileAlpha(const int _tile_index, const float _alpha);
    void modifyTileAlpha(const int _tile_index, const float _amount);
    void setTileColor(const int _tile_index, const sf::Color& _color);

    void draw(sf::RenderWindow& _window);

private:
    void init(const sf::Color& _initial_color);

    int size_x;
    int size_y;

    float tile_width;
    float tile_height;

    std::vector<Tile> tiles;

};
