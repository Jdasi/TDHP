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

    Heatmap* createHeatmap(const sf::Color& _color, const float _paint_hardness,
        const float _decay_rate, const HeatmapFlag& _flag,
        const WeightingType& _weighting_type = WeightingType::POSITIVE);

    void paintOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius);
    void splashOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius);

    int getAllWeights(const int _tile_index);
    int getWeights(const int _tile_index, const int _flags);

private:
    std::vector<std::unique_ptr<Heatmap>> heatmaps;

    Level& level;

};
