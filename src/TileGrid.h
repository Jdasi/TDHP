#pragma once

#include "Tile.h"

#include <SFML/Graphics.hpp>

#include <vector>

class TileGrid
{
public:
    TileGrid(const int _size_x, const int _size_y, const float _padding,
        const sf::Color& _initial_color);

    ~TileGrid() = default;

    int getSizeX() const;
    int getSizeY() const;
    int getProduct() const;

    void setTileAlpha(const int _tile_index, const float _alpha);
    void setTileAlpha(const sf::Vector2f& _pos, const float _alpha);

    void modifyTileAlpha(const int _tile_index, const float _amount);
    void modifyTileAlpha(const sf::Vector2f& _pos, const float _amount);

    void setTileColor(const int _tile_index, const sf::Color& _color);
    void setTileColor(const sf::Vector2f& _pos, const sf::Color& _color);

    void draw(sf::RenderWindow& _window);

private:
    void init(const sf::Color& _initial_color);
    bool validIndex(const int _tile_index);
    Tile* getOverlappingTile(const sf::Vector2f& _pos);

    int size_x;
    int size_y;
    float padding;

    std::vector<Tile> tiles;

};
