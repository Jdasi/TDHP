#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

#include "TileGrid.h"

class HeatMap
{
public:
    HeatMap(const int _size_x, const int _size_y);
    ~HeatMap() = default;

    void resetGrid(const int _size_x, const int _size_y);

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

private:
    void paintWithModifier(const int _tile_index, const int _radius,
        const float _modifier = 1);

    void decay();

    bool active;
    bool visible;

    sf::Color color;

    float paint_hardness;
    float decay_rate;

    std::vector<float> weightings;
    std::unique_ptr<TileGrid> grid;

};
