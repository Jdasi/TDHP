#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Heatmap.h"

class HeatmapManager
{
public:
    HeatmapManager(const int _grid_size_x, const int _grid_size_y);
    ~HeatmapManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    Heatmap* createHeatmap(const sf::Color& _color,
        const float _paint_hardness, const float _decay_rate);

    void paintOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius);
    void splashOnHeatmap(const int _heatmap_index, const int _tile_index, const int _radius);

private:
    std::vector<std::unique_ptr<Heatmap>> heat_maps;

    int grid_size_x;
    int grid_size_y;

};
