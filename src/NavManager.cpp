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


HeatMap* NavManager::createHeatMap(const sf::Color& _hot_color, const sf::Color& _cold_color,
    const float _paint_hardness, const float _decay_rate)
{
    std::unique_ptr<HeatMap> heat_map = std::make_unique<HeatMap>(GRID_SIZE_X, GRID_SIZE_Y, TILE_PADDING);
    HeatMap* heat_map_ptr = heat_map.get();

    heat_map->setColors(sf::Color::Red, sf::Color(255, 0, 0, 0));
    heat_map->setPaintHardness(_paint_hardness);
    heat_map->setDecayRate(_decay_rate);

    heat_maps.push_back(std::move(heat_map));
    return heat_map_ptr;
}


void NavManager::toggleTileWalkable(const sf::Vector2f& _pos)
{
    // TODO: toggle ..
    grid.setTileColor(_pos, BLOCKED_COLOR);
}


void NavManager::tick()
{
    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isActive())
            heat_map->tick();
    }
}


void NavManager::draw(sf::RenderWindow& _window)
{
    _window.draw(border);
    grid.draw(_window);

    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isVisible())
            heat_map->draw(_window);
    }
}
