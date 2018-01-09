#pragma once

#include <vector>
#include <memory>

#include "Heatmap.h"

namespace sf
{
    class RenderWindow;
    class Color;
}

class Level;

class HeatmapManager
{
public:
    HeatmapManager(Level& _level);
    ~HeatmapManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void paintOnHeatmap(const HeatmapFlag& _flags, const int _tile_index, const int _radius);
    void paintOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius);

    void splashOnHeatmap(const HeatmapFlag& _flags, const int _tile_index, const int _radius);
    void splashOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius);

    int getWeight(const int _tile_index, const HeatmapFlag& _flags = HeatmapFlag::ALL);
    int getTotalWeight(const HeatmapFlag& _flags = HeatmapFlag::ALL);

private:
    struct HeatmapPair
    {
        HeatmapFlag key;
        std::unique_ptr<Heatmap> heatmap;
    };

    void initHeatmaps();
    void createHeatmap(const HeatmapFlag& _flag, const sf::Color& _color,
        const float _paint_hardness, const float _decay_rate,
        const WeightingType& _weighting_type = WeightingType::POSITIVE);

    bool heatmapExists(const HeatmapFlag& _key);
    Heatmap* findHeatmap(const HeatmapFlag& _key);

    int calculateWeight(const HeatmapFlag& _flags, const int _tile_index = -1);

    std::vector<HeatmapPair> heatmap_entries;

    Level& level;

};
