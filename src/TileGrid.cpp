#include "TileGrid.h"
#include "JHelper.h"
#include "Constants.h"


TileGrid::TileGrid(const int _size_x, const int _size_y, const float _padding,
    const sf::Color& _initial_color)
    : size_x(_size_x)
    , size_y(_size_y)
    , padding(_padding)
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


int TileGrid::getProduct() const
{
    return size_x * size_y;
}


void TileGrid::setTileAlpha(const int _tile_index, const float _alpha)
{
    if (!validIndex(_tile_index))
        return;

    auto& tile = tiles[_tile_index];
    auto& color = tile.getFillColor();
    tile.setFillColor(sf::Color(color.r, color.g, color.b, _alpha));
}


void TileGrid::setTileAlpha(const sf::Vector2f& _pos, const float _alpha)
{
    Tile* tile = getOverlappingTile(_pos);
    if (!tile)
        return;

    auto& color = tile->getFillColor();
    tile->setFillColor(sf::Color(color.r, color.g, color.b, _alpha));
}


void TileGrid::modifyTileAlpha(const int _tile_index, const float _amount)
{
    if (!validIndex(_tile_index))
        return;

    auto& tile = tiles[_tile_index];
    auto& color = tile.getFillColor();
    tile.setFillColor(sf::Color(color.r, color.g, color.b, color.a + _amount));
}


void TileGrid::modifyTileAlpha(const sf::Vector2f& _pos, const float _amount)
{
    Tile* tile = getOverlappingTile(_pos);
    if (!tile)
        return;

    auto& color = tile->getFillColor();
    tile->setFillColor(sf::Color(color.r, color.g, color.b, color.a + _amount));
}


void TileGrid::setTileColor(const int _tile_index, const sf::Color& _color)
{
    if (!validIndex(_tile_index))
        return;

    tiles[_tile_index].setFillColor(_color);
}


void TileGrid::setTileColor(const sf::Vector2f& _pos, const sf::Color& _color)
{
    Tile* tile = getOverlappingTile(_pos);
    if (!tile)
        return;

    tile->setFillColor(_color);
}


void TileGrid::draw(sf::RenderWindow& _window)
{
    for (auto& sprite : tiles)
    {
        if (sprite.getFillColor().a <= 0)
            continue;

        _window.draw(sprite);
    }
}


void TileGrid::init(const sf::Color& _initial_color)
{
    tiles.assign(size_x * size_y, Tile());

    float rect_width = (PANE_WIDTH - ((size_x - 1) * padding)) / size_x;
    float rect_height = (PANE_HEIGHT - ((size_y - 1) * padding)) / size_y;
    sf::Vector2f rect({ rect_width, rect_height });

    for (int y_cycles = 0; y_cycles < size_y; ++y_cycles)
    {
        for (int x_cycles = 0; x_cycles < size_x; ++x_cycles)
        {
            auto& tile = tiles[JHelper::calculateIndex(x_cycles, y_cycles, size_x)];

            tile.setCoords(sf::Vector2i(x_cycles, y_cycles));
            tile.setSize(rect);
            tile.setFillColor(_initial_color);
            tile.setPosition({ WINDOW_LEFT_BOUNDARY + (x_cycles * (rect.x + padding)),
                WINDOW_TOP_BOUNDARY + (y_cycles * (rect.y + padding)) });
        }
    }
}


bool TileGrid::validIndex(const int _tile_index)
{
    if (_tile_index < 0 || _tile_index >= tiles.size())
        return false;

    return true;
}


// If any tile contains _pos, it is returned. Otherwise returns nullptr.
Tile* TileGrid::getOverlappingTile(const sf::Vector2f& _pos)
{
    for (auto& tile : tiles)
    {
        if (!tile.getGlobalBounds().contains(_pos))
            continue;

        return &tile;
    }

    return nullptr;
}
