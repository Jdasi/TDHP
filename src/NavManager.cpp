#include "NavManager.h"
#include "JHelper.h"
#include "Constants.h"


NavManager::NavManager()
    : grid(GRID_SIZE_X, GRID_SIZE_Y, TILE_PADDING, WALKABLE_COLOR)
{
    nav_nodes.assign(GRID_SIZE, NavNode());

    border.setPosition({ WINDOW_LEFT_BOUNDARY, WINDOW_TOP_BOUNDARY });
    border.setSize(PANE_SIZE);
    border.setFillColor(sf::Color::Blue);
    border.setOutlineThickness(5.0f);
}


void NavManager::colorTileAtPos(const sf::Vector2f& _pos)
{
    //for (sf::RectangleShape& tile : tiles)
    //{
    //    if (!tile.getGlobalBounds().contains(_pos))
    //        continue;

    //    tile.setFillColor(BLOCKED_COLOR);
    //    break;
    //}
}


void NavManager::tick()
{
    for (HeatMap heat_map : heat_maps)
    {
        if (heat_map.isActive())
            heat_map.tick();
    }
}


void NavManager::draw(sf::RenderWindow& _window)
{
    _window.draw(border);
    grid.draw(_window);

    for (HeatMap heat_map : heat_maps)
    {
        if (heat_map.isVisible())
            heat_map.draw(_window);
    }
}
