#include "NavManager.h"
#include "JHelper.h"
#include "Constants.h"
#include "Level.h"

#include <iostream>


NavManager::NavManager()
    : grid(nullptr)
{
}


void NavManager::parseLevel(const Level& _level)
{
    int width = _level.width;
    int height = _level.height;

    nav_nodes.assign(width * height, NavNode());
    grid = std::make_unique<TileGrid>(width, height, WALKABLE_COLOR);

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int index = JHelper::calculateIndex(col, row, width);
            nav_nodes[index].setWorldPos(grid->tileIndexToPos(index));

            switch (_level.data[index])
            {
                case 'W': toggleTileWalkable(index); break;
            }
        }
    }

    for (auto& heat_map : heat_maps)
    {
        heat_map->resetGrid(width, height);
    }
}


HeatMap* NavManager::createHeatMap(const sf::Color& _color,
    const float _paint_hardness, const float _decay_rate)
{
    auto heat_map = std::make_unique<HeatMap>(grid->getSizeX(), grid->getSizeY());
    HeatMap* heat_map_ptr = heat_map.get();

    heat_map->setColor(_color);
    heat_map->setPaintHardness(_paint_hardness);
    heat_map->setDecayRate(_decay_rate);

    heat_maps.push_back(std::move(heat_map));
    return heat_map_ptr;
}


bool NavManager::isTileWalkable(const int _index) const
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return false;

    return nav_nodes[_index].isWalkable();
}


void NavManager::toggleTileWalkable(const sf::Vector2f& _pos)
{
    int tile_index = grid->posToTileIndex(_pos);
    if (tile_index == TileGrid::INVALID_TILE)
        return;

    toggleTileWalkable(tile_index);
}


void NavManager::toggleTileWalkable(const int _index)
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return;

    auto& nav_node = nav_nodes[_index];
    nav_node.toggleWalkable();

    grid->setTileColor(_index, nav_node.isWalkable() ? WALKABLE_COLOR : UNWALKABLE_COLOR);
}


void NavManager::paintOnHeatMap(const int _heatmap_index, const int _tile_index, const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heat_maps.size()))
        return;

    heat_maps[_heatmap_index]->paint(_tile_index, _radius);
}


sf::Vector2f NavManager::getTileWorldPos(const int _tile_index)
{
    sf::Vector2f pos;
    if (!JHelper::validIndex(_tile_index, nav_nodes.size()))
        return pos;

    return nav_nodes[_tile_index].getWorldPos();
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
    grid->draw(_window);
}


void NavManager::drawHeatMaps(sf::RenderWindow& _window)
{
    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isVisible())
            heat_map->draw(_window);
    }
}
