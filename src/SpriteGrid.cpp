#include "SpriteGrid.h"
#include "JHelper.h"
#include "Constants.h"


SpriteGrid::SpriteGrid(const int _size_x, const int _size_y, const float _padding,
    const sf::Color& _initial_color)
    : size_x(_size_x)
    , size_y(_size_y)
    , padding(_padding)
{
    init(_initial_color);
}


void SpriteGrid::draw(sf::RenderWindow& _window)
{
    for (sf::RectangleShape sprite : sprites)
    {
        if (sprite.getFillColor().a <= 0)
            continue;

        _window.draw(sprite);
    }
}


void SpriteGrid::init(const sf::Color& _initial_color)
{
    sprites.assign(size_x * size_y, sf::RectangleShape());

    float rect_width = (PANE_WIDTH - ((size_x - 1) * padding)) / size_x;
    float rect_height = (PANE_HEIGHT - ((size_y - 1) * padding)) / size_y;
    sf::Vector2f rect({ rect_width, rect_height });

    for (int y_cycles = 0; y_cycles < size_y; ++y_cycles)
    {
        for (int x_cycles = 0; x_cycles < size_x; ++x_cycles)
        {
            auto& tile = sprites[JHelper::calculateIndex(x_cycles, y_cycles, size_x)];

            tile.setSize(rect);
            tile.setFillColor(_initial_color);
            tile.setPosition({ WINDOW_LEFT_BOUNDARY + (x_cycles * (rect.x + padding)),
                WINDOW_TOP_BOUNDARY + (y_cycles * (rect.y + padding)) });
        }
    }
}
