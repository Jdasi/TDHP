#include <SFML/Graphics/RenderWindow.hpp>

#include "TileGrid.h"
#include "JHelper.h"
#include "Constants.h"
#include "Level.h"


TileGrid::TileGrid(const Level& _level, const sf::Color& _initial_color)
    : size_x(_level.getSizeX())
    , size_y(_level.getSizeY())
    , tile_width(_level.getTileWidth())
    , tile_height(_level.getTileHeight())
{
    init(_initial_color);
}


int TileGrid::getSizeX() const
{
    return size_x;
}


int TileGrid::getSizeY() const
{
    return size_y;
}


void TileGrid::setTileAlpha(const int _tile_index, const float _alpha)
{
    if (!JHelper::validIndex(_tile_index, tiles.size()))
        return;

    auto& tile = tiles[_tile_index];
    auto& color = tile.getFillColor();

    tile.setFillColor(sf::Color(color.r, color.g, color.b, _alpha));
}


void TileGrid::modifyTileAlpha(const int _tile_index, const float _amount)
{
    if (!JHelper::validIndex(_tile_index, tiles.size()))
        return;

    auto& tile = tiles[_tile_index];
    auto& color = tile.getFillColor();

    tile.setFillColor(sf::Color(color.r, color.g, color.b, color.a + _amount));
}


void TileGrid::setTileColor(const int _tile_index, const sf::Color& _color)
{
    if (!JHelper::validIndex(_tile_index, tiles.size()))
        return;

    tiles[_tile_index].setFillColor(_color);
}


void TileGrid::draw(sf::RenderWindow& _window)
{
    for (auto& tile : tiles)
    {
        if (tile.getFillColor().a <= 0)
            continue;

        _window.draw(tile);
    }
}


/* Tiles are squished to fit inside the PANE, and are set to have their origins
 * in the center, so that other centered sprites can be easily layered on top.
 */
void TileGrid::init(const sf::Color& _initial_color)
{
    tiles.assign(size_x * size_y, Tile());

    float half_rect_x = tile_width / 2;
    float half_rect_y = tile_height / 2;

    sf::Vector2f rect({ tile_width, tile_height });

    for (int row = 0; row < size_y; ++row)
    {
        for (int col = 0; col < size_x; ++col)
        {
            auto& tile = tiles[JHelper::calculateIndex(col, row, size_x)];

            tile.setCoords(sf::Vector2i(col, row));
            tile.setSize(rect);
            tile.setOrigin(half_rect_x, half_rect_y);
            tile.setFillColor(_initial_color);
            tile.setPosition({
                WINDOW_LEFT_BOUNDARY + (col * (rect.x)) + half_rect_x,
                WINDOW_TOP_BOUNDARY + (row * (rect.y)) + half_rect_y });
        }
    }
}
