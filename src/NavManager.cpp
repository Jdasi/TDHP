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


HeatMap* NavManager::createHeatMap(const sf::Color& _color,
    const float _paint_hardness, const float _decay_rate)
{
    auto heat_map = std::make_unique<HeatMap>(GRID_SIZE_X, GRID_SIZE_Y, TILE_PADDING);
    HeatMap* heat_map_ptr = heat_map.get();

    heat_map->setColor(_color);
    heat_map->setPaintHardness(_paint_hardness);
    heat_map->setDecayRate(_decay_rate);

    heat_maps.push_back(std::move(heat_map));
    return heat_map_ptr;
}


void NavManager::toggleTileWalkable(const sf::Vector2f& _pos)
{
    int tile_index = grid.posToTileIndex(_pos);
    if (tile_index == TileGrid::INVALID_TILE)
        return;

    auto& nav_node = nav_nodes[tile_index];
    nav_node.toggleWalkable();

    grid.setTileColor(tile_index, nav_node.isWalkable() ? WALKABLE_COLOR : UNWALKABLE_COLOR);
}


void NavManager::paintOnHeatMap(const int _heatmap_index, const sf::Vector2f& _pos, const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heat_maps.size()))
        return;

    heat_maps[_heatmap_index]->paint(_pos, _radius);
}


void NavManager::tick()
{
    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isActive())
            heat_map->tick();
    }
}


void NavManager::drawBaseLayer(sf::RenderWindow& _window)
{
    _window.draw(border);
    grid.draw(_window);
}


void NavManager::drawHeatMaps(sf::RenderWindow& _window)
{
    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isVisible())
            heat_map->draw(_window);
    }
}
