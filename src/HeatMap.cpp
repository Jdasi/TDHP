#include "HeatMap.h"
#include "JTime.h"
#include "JMath.h"
#include "JHelper.h"


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


void HeatMap::paint(const sf::Vector2f& _pos, const int _radius)
{
    int center_tile = grid.posToTileIndex(_pos);
    if (center_tile == TileGrid::INVALID_TILE)
        return;

    int size_x = grid.getSizeX();
    int size_y = grid.getSizeY();

    sf::Vector2i coords = JHelper::calculateCoords(center_tile, size_x);

    // Paint everything including and around coords.
    for (int row = coords.y - _radius; row <= coords.y + _radius; ++row)
    {
        for (int col = coords.x - _radius; col <= coords.x + _radius; ++col)
        {
            if ((col < 0 || col >= size_x) || (row < 0 || row >= size_y))
                continue;

            int curr = JHelper::calculateIndex(col, row, size_x);
            int diff = abs(col - coords.x) + abs(row - coords.y);

            auto& weighting = weightings[curr];

            weighting += (paint_hardness / (diff + 1)) * JTime::getUnscaledDeltaTime();
            weighting = JMath::clampf(weighting, 0, 255);

            grid.setTileAlpha(curr, weighting);
        }
    }
}


void HeatMap::setColor(const sf::Color& _color)
{
    color = _color;

    // Reset weightings.
    for (int i = 0; i < weightings.size(); ++i)
    {
        grid.setTileColor(i, _color);
        grid.setTileAlpha(i, 0);

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
