#pragma once

#include <vector>

#include <SFML/Graphics/Color.hpp>

#include "TileGrid.h"
#include "HeatmapFlag.h"
#include "WeightingType.h"

namespace sf
{
    class RenderWindow;
}

class Level;

class Heatmap
{
public:
    Heatmap(const Level& _level, const HeatmapFlag& _flag,
        const WeightingType& _weighting_type = WeightingType::POSITIVE);

    ~Heatmap() = default;

    bool isActive() const;
    void setActive(const bool _active);

    bool isVisible() const;
    void setVisible(const bool _visible);

    void setPaintHardness(const float _hardness);
    void setDecayRate(const float _decay_rate);
    void setColor(const sf::Color& _color);

    void paint(const int _tile_index, const int _radius);
    void splash(const int _tile_index, const int _radius);

    void tick();
    void draw(sf::RenderWindow& _window);

    int getWeight(const int _tile_index);
    int getTotalWeight();
    HeatmapFlag getFlag() const;

    void setWeightingType(const WeightingType& _weighting_type);
    WeightingType getWeightingType() const;

    int getHighestWeightIndex() const;

private:
    void paintWithModifier(const int _tile_index, const int _radius,
        const float _modifier = 1);

    void decay();
    void updateHighestWeighting(const float _weighting, const int _index);

    HeatmapFlag flag;
    WeightingType weighting_type;

    bool active;
    bool visible;

    sf::Color color;

    float paint_hardness;
    float decay_rate;

    std::vector<float> weightings;
    TileGrid grid;
    float total_weight;

    float highest_weight;
    int highest_weight_index;

};
