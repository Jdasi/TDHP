#include <algorithm>

#include "HeatmapManager.h"
#include "JHelper.h"
#include "Level.h"


HeatmapManager::HeatmapManager(Level& _level)
    : level(_level)
{
    initHeatmaps();
}


void HeatmapManager::tick()
{
    for (auto& entry : heatmap_entries)
    {
        if (entry.heatmap->isActive())
            entry.heatmap->tick();
    }
}


void HeatmapManager::draw(sf::RenderWindow& _window)
{
    for (auto& entry : heatmap_entries)
    {
        if (entry.heatmap->isVisible())
            entry.heatmap->draw(_window);
    }
}


void HeatmapManager::paintOnHeatmap(const HeatmapFlag& _flags, const int _tile_index,
    const int _radius)
{
    for (auto& entry : heatmap_entries)
    {
        if (!(entry.heatmap->getFlag() & _flags))
            continue;

        entry.heatmap->paint(_tile_index, _radius);
    }
}


void HeatmapManager::paintOnHeatmap(const int _heatmap_index, const int _tile_index,
    const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heatmap_entries.size()))
        return;

    heatmap_entries[_heatmap_index].heatmap->paint(_tile_index, _radius);
}


void HeatmapManager::splashOnHeatmap(const HeatmapFlag& _flags, const int _tile_index,
    const int _radius)
{
    for (auto& entry : heatmap_entries)
    {
        if (!(entry.heatmap->getFlag() & _flags))
            continue;

        entry.heatmap->splash(_tile_index, _radius);
    }
}


void HeatmapManager::splashOnHeatmap(const int _heatmap_index, const int _tile_index,
    const int _radius)
{
    if (!JHelper::validIndex(_heatmap_index, heatmap_entries.size()))
        return;

    heatmap_entries[_heatmap_index].heatmap->splash(_tile_index, _radius);
}


int HeatmapManager::getWeight(const int _tile_index, const HeatmapFlag& _flags,
    const bool _abs) const
{
    return calculateWeight(_flags, _tile_index, _abs);
}


int HeatmapManager::getTotalWeight(const HeatmapFlag& _flags, const bool _abs) const
{
    return calculateWeight(_flags, -1, _abs);
}


int HeatmapManager::getHighestWeightIndex(const HeatmapFlag& _flags) const
{
    int highest_weight = 0;
    int highest_index = 0;

    for (auto& entry : heatmap_entries)
    {
        if (!entry.heatmap->isActive() || !(entry.heatmap->getFlag() & _flags))
            continue;

        int index = entry.heatmap->getHighestWeightIndex();
        int weight = entry.heatmap->getWeight(index);

        if (weight <= highest_weight)
            continue;

        highest_weight = weight;
        highest_index = index;
    }

    return highest_index;
}


void HeatmapManager::initHeatmaps()
{
    createHeatmap(HeatmapFlag::LASER_DEATHS,  sf::Color::Blue,    200,   5);
    createHeatmap(HeatmapFlag::BULLET_DEATHS, sf::Color::Red,     200,   5);
    createHeatmap(HeatmapFlag::SMOKE,         sf::Color::Black,   1200, 10, WeightingType::NEGATIVE);
    createHeatmap(HeatmapFlag::DEBUG,         sf::Color::Magenta, 200,   5);

    // Sort heatmaps by their flag.
    std::sort(heatmap_entries.begin(), heatmap_entries.end(),
        [](const auto& _lhs, const auto& _rhs)
    {
        return _lhs.key < _rhs.key;
    });
}


void HeatmapManager::createHeatmap(const HeatmapFlag& _key, const sf::Color& _color,
    const float _paint_hardness, const float _decay_rate, const WeightingType& _weighting_type)
{
    if (heatmapExists(_key))
        return;

    HeatmapPair entry;

    entry.key = _key;
    entry.heatmap = std::make_unique<Heatmap>(level, _key, _weighting_type);

    entry.heatmap->setColor(_color);
    entry.heatmap->setPaintHardness(_paint_hardness);
    entry.heatmap->setDecayRate(_decay_rate);

    heatmap_entries.push_back(std::move(entry));
}


bool HeatmapManager::heatmapExists(const HeatmapFlag& _key) const
{
    auto heatmap = findHeatmap(_key);
    return heatmap != nullptr;
}


Heatmap* HeatmapManager::findHeatmap(const HeatmapFlag& _key) const
{
    auto entry = std::find_if(heatmap_entries.begin(), heatmap_entries.end(),
        [_key](const auto& _elem)
    {
        return _elem.key == _key;
    });

    if (entry != heatmap_entries.end())
    {
        return entry->heatmap.get();
    }

    return nullptr;
}


/* Calculates the weight of any heatmaps that match the passed flags.
 * If _tile_index is >= 0, the weight of a single tile is calculated,
 * otherwise the total weight of relevant heatmaps are calculated.
 *
 * If _abs is true, NEGATIVE WeightingTypes will be forced to POSITIVE
 * for this calculation.
 */
int HeatmapManager::calculateWeight(const HeatmapFlag& _flags, const int _tile_index,
    const bool _abs) const
{
    int weight = 0;

    for (auto& entry : heatmap_entries)
    {
        if (!entry.heatmap->isActive() || !(entry.heatmap->getFlag() & _flags))
            continue;

        // Get either the weight of a single tile or the whole heatmap's weight.
        int weighting = _tile_index >= 0 ?
            entry.heatmap->getWeight(_tile_index) : entry.heatmap->getTotalWeight();

        // Heatmaps can have have an overall positive or negative weight contribution.
        weight += (entry.heatmap->getWeightingType() == WeightingType::POSITIVE || _abs) ?
            weighting : -weighting;
    }

    // Weights must not be negative to work correctly with A*.
    if (weight < 0)
        weight = 0;

    return weight;
}
