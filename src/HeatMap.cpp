#include "HeatMap.h"
#include "Constants.h"


HeatMap::HeatMap()
    : active(true)
    , visible(false)
    , grid(GRID_SIZE_X, GRID_SIZE_Y, TILE_PADDING, sf::Color::Transparent)
{
}


bool HeatMap::isActive() const
{
    return active;
}


void HeatMap::setActive(const bool _active)
{
    active = _active;
}


bool HeatMap::isVisible() const
{
    return visible;
}


void HeatMap::setVisible(const bool _visible)
{
    visible = _visible;
}


void HeatMap::setPaintHardness(const float _hardness)
{
    paint_hardness = _hardness;
}


void HeatMap::setDecayRate(const float _decay_rate)
{
    decay_rate = _decay_rate;
}


void HeatMap::paint(const sf::Vector2i& _pos, int _radius)
{
    // TODO: paint using hardness according to _radius ...
}


void HeatMap::tick()
{
    decay();
}


void HeatMap::draw(sf::RenderWindow& _window)
{
    grid.draw(_window);
}


void HeatMap::decay()
{
    if (decay_rate <= 0)
        return;

    // TODO: fade out over time ...
}
