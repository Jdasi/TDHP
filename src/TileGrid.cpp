#include "TileGrid.h"
#include "JHelper.h"
#include "Constants.h"


TileGrid::TileGrid(const int _size_x, const int _size_y, const sf::Color& _initial_color)
    : size_x(_size_x)
    , size_y(_size_y)
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


void TileGrid::setTileAlpha(const int _tile_index, const int _alpha)
{
    if (!JHelper::validIndex(_tile_index, tiles.size()))
        return;

    auto& tile = tiles[_tile_index];
    auto& color = tile.getFillColor();

    tile.setFillColor(sf::Color(color.r, color.g, color.b, _alpha));
}


void TileGrid::modifyTileAlpha(const int _tile_index, const int _amount)
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
    for (auto& sprite : tiles)
    {
        if (sprite.getFillColor().a <= 0)
            continue;

        _window.draw(sprite);
    }
}


/* Tiles are squished to fit inside the PANE, and are set to have their origins
 * in the center, to allow for other sprites to be more easily layered on top.
 */
void TileGrid::init(const sf::Color& _initial_color)
{
    tiles.assign(size_x * size_y, Tile());

    float rect_width = PANE_WIDTH / size_x;
    float rect_height = PANE_HEIGHT / size_y;

    float half_rect_x = rect_width / 2;
    float half_rect_y = rect_height / 2;

    sf::Vector2f rect({ rect_width, rect_height });

    for (int y_cycles = 0; y_cycles < size_y; ++y_cycles)
    {
        for (int x_cycles = 0; x_cycles < size_x; ++x_cycles)
        {
            auto& tile = tiles[JHelper::calculateIndex(x_cycles, y_cycles, size_x)];

            tile.setCoords(sf::Vector2i(x_cycles, y_cycles));
            tile.setSize(rect);
            tile.setOrigin(half_rect_x, half_rect_y);
            tile.setFillColor(_initial_color);
            tile.setPosition({
                WINDOW_LEFT_BOUNDARY + (x_cycles * (rect.x)) + half_rect_x,
                WINDOW_TOP_BOUNDARY + (y_cycles * (rect.y)) + half_rect_y });
        }
    }
}


int TileGrid::posToTileIndex(const sf::Vector2f& _pos)
{
    int index = 0;
    for (auto& tile : tiles)
    {
        if (!tile.getGlobalBounds().contains(_pos))
        {
            ++index;
            continue;
        }

        return index;
    }

    return ReturnType::INVALID_TILE;
}


sf::Vector2f TileGrid::tileIndexToPos(const int _tile_index)
{
    sf::Vector2f pos;
    if (!JHelper::validIndex(_tile_index, tiles.size()))
        return pos;

    return tiles[_tile_index].getPosition();
}
