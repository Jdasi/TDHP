#pragma once

#include "TileGrid.h"

#include <SFML/Graphics.hpp>

#include <vector>

class HeatMap
{
public:
    HeatMap(const int _size_x, const int _size_y, const int _padding);
    ~HeatMap() = default;

    bool isActive() const;
    void setActive(const bool _active);

    bool isVisible() const;
    void setVisible(const bool _visible);

    void setPaintHardness(const float _hardness);
    void setDecayRate(const float _decay_rate);
    void setColor(const sf::Color& _color);

    void paint(const sf::Vector2f& _pos, const int _radius);

    void tick();
    void draw(sf::RenderWindow& _window);

private:
    void decay();

    bool active;
    bool visible;

    sf::Color color;

    float paint_hardness;
    float decay_rate;

    std::vector<float> weightings;
    TileGrid grid;

};
