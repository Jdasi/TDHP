#include "HeatmapManager.h"
#include "JHelper.h"
#include "Level.h"


HeatmapManager::HeatmapManager(Level& _level)
    : level(_level)
{
    createHeatmap(sf::Color::Blue, 200, 15, HeatmapFlag::LASER_DEATHS);
    createHeatmap(sf::Color::Red, 200, 15, HeatmapFlag::BULLET_DEATHS);
    createHeatmap(sf::Color::White, 200, 15, HeatmapFlag::TURRET_SHOTS, WeightingType::NEGATIVE);
    createHeatmap(sf::Color::Magenta, 200, 15, HeatmapFlag::DEBUG);
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
        if (heatmap->isVisible())
            heatmap->draw(_window);
    }
}


Heatmap* HeatmapManager::createHeatmap(const sf::Color& _color,
    const float _paint_hardness, const float _decay_rate, const HeatmapFlag& _flag,
    const WeightingType& _weighting_type)
{
    auto heatmap = std::make_unique<Heatmap>(level, _flag, _weighting_type);
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
    return getWeights(_tile_index, HeatmapFlag::ALL);
}


int HeatmapManager::getWeights(const int _tile_index, const int _flags)
{
    int weight = 0;

    for (auto& heatmap : heatmaps)
    {
        if (!heatmap->isActive() || !(heatmap->getFlag() & _flags))
            continue;

            int weighting = heatmap->getWeight(_tile_index);

            weight += heatmap->getWeightingType() == WeightingType::POSITIVE ?
                weighting : -weighting;
    }

    // Weights must not be negative to work correctly with A*.
    if (weight < 0)
        weight = 0;

    return weight;
}


int HeatmapManager::getHeatmapTotalWeight(const int _heatmap_index)
{
    if (!JHelper::validIndex(_heatmap_index, heatmaps.size()))
        return 0;

    return heatmaps[_heatmap_index]->getTotalWeight();
}
