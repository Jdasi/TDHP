#include "HeatMap.h"
#include "JTime.h"
#include "JMath.h"


HeatMap::HeatMap(const int _size_x, const int _size_y, const int _padding)
    : active(true)
    , visible(true)
    , paint_hardness(0)
    , decay_rate(0)
    , grid(_size_x, _size_y, _padding, sf::Color::Transparent)
{
    weightings.assign(_size_x * _size_y, 0);
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


void HeatMap::paint(const sf::Vector2f& _pos, int _radius)
{
    float dt = JTime::getUnscaledDeltaTime();
    grid.modifyTileAlpha(_pos, paint_hardness * dt);
}


void HeatMap::setColors(const sf::Color& _hot_color, const sf::Color& _cold_color)
{
    hot_color = _hot_color;
    cold_color = _cold_color;

    for (int i = 0; i < weightings.size(); ++i)
    {
        grid.setTileColor(i, _cold_color);
        weightings[i] = 0;
    }
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

    for (int i = 0; i < weightings.size(); ++i)
    {
        float& weighting = weightings[i];

        if (weighting > 0)
        {
            weighting -= decay_rate * JTime::getDeltaTime();
        }

        weighting = JMath::clampf(weighting, 0, 255);
        grid.setTileAlpha(i, weighting);
    }
}
