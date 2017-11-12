#pragma once

#include "Tile.h"

#include <SFML/Graphics.hpp>

#include <vector>

class TileGrid
{
public:
    enum ReturnType : int
    {
        INVALID_TILE = -1
    };

    TileGrid(const int _size_x, const int _size_y, const float _padding,
        const sf::Color& _initial_color);

    ~TileGrid() = default;

    int getSizeX() const;
    int getSizeY() const;
    int getProduct() const;

    int posToTileIndex(const sf::Vector2f& _pos);

    void setTileAlpha(const int _tile_index, const float _alpha);
    void modifyTileAlpha(const int _tile_index, const float _amount);
    void setTileColor(const int _tile_index, const sf::Color& _color);

    void draw(sf::RenderWindow& _window);

private:
    void init(const sf::Color& _initial_color);
    bool validIndex(const int _tile_index);

    int size_x;
    int size_y;
    float padding;

    std::vector<Tile> tiles;

};
