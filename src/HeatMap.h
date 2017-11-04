#pragma once

#include "SpriteGrid.h"

#include <SFML/Graphics.hpp>

#include <vector>

class HeatMap
{
public:
    HeatMap();
    ~HeatMap() = default;

    bool isActive() const;
    void setActive(const bool _active);

    bool isVisible() const;
    void setVisible(const bool _visible);

    void setPaintHardness(const float _hardness);
    void setDecayRate(const float _decay_rate);
    void paint(const sf::Vector2i& _pos, int _radius);

    void tick();
    void draw(sf::RenderWindow& _window);

private:
    void decay();

    bool active;
    bool visible;

    float paint_hardness;
    float decay_rate;
    std::vector<float> weightings;
    SpriteGrid grid;

};
