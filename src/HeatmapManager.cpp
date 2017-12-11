#include "HeatmapManager.h"
#include "JHelper.h"
#include "Level.h"


HeatmapManager::HeatmapManager(Level& _level)
    : level(_level)
{
}


void HeatmapManager::tick()
{
    for (auto& heatmap : heatmaps)
    {
        if (heatmap->isActive())
            heatmap->tick();
    }
}


void HeatmapManager::draw(sf::RenderWindow& _window)
{
    for (auto& heatmap : heatmaps)
    {
        if (heatmap->isActive())
            heatmap->draw(_window);
    }
}


Heatmap* HeatmapManager::createHeatmap(const sf::Color& _color,
    const float _paint_hardness, const float _decay_rate)
{
    auto heatmap = std::make_unique<Heatmap>(level);
    auto* heat_map_ptr = heatmap.get();

    heatmap->setColor(_color);
    heatmap->setPaintHardness(_paint_hardness);
    heatmap->setDecayRate(_decay_rate);

    heatmaps.push_back(std::move(heatmap));
    return heat_map_ptr;
}


void HeatmapManager::paintOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heatmaps.size()))
        return;

    heatmaps[_heatmap_index]->paint(_tile_index, _radius);
}


void HeatmapManager::splashOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heatmaps.size()))
        return;

    heatmaps[_heatmap_index]->splash(_tile_index, _radius);
}


int HeatmapManager::getAllWeights(const int _tile_index)
{
    int weight = 0;

    for (auto& heatmap : heatmaps)
    {
        weight += heatmap->getWeight(_tile_index);
    }

    return weight;
}
