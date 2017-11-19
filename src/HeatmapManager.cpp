#include "HeatmapManager.h"
#include "JHelper.h"


HeatmapManager::HeatmapManager(const int _grid_size_x, const int _grid_size_y)
    : grid_size_x(_grid_size_x)
    , grid_size_y(_grid_size_y)
{
}


void HeatmapManager::tick()
{
    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isActive())
            heat_map->tick();
    }
}


void HeatmapManager::draw(sf::RenderWindow& _window)
{
    for (auto& heat_map : heat_maps)
    {
        if (heat_map->isActive())
            heat_map->draw(_window);
    }
}


Heatmap* HeatmapManager::createHeatmap(const sf::Color& _color,
    const float _paint_hardness, const float _decay_rate)
{
    auto heat_map = std::make_unique<Heatmap>(grid_size_x, grid_size_y);
    Heatmap* heat_map_ptr = heat_map.get();

    heat_map->setColor(_color);
    heat_map->setPaintHardness(_paint_hardness);
    heat_map->setDecayRate(_decay_rate);

    heat_maps.push_back(std::move(heat_map));
    return heat_map_ptr;
}


void HeatmapManager::paintOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heat_maps.size()))
        return;

    heat_maps[_heatmap_index]->paint(_tile_index, _radius);
}


void HeatmapManager::splashOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heat_maps.size()))
        return;

    heat_maps[_heatmap_index]->splash(_tile_index, _radius);
}
