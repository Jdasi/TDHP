#include "Heatmap.h"
#include "JTime.h"
#include "JMath.h"
#include "JHelper.h"
#include "Level.h"
#include "Constants.h"


Heatmap::Heatmap(const Level& _level, const HeatmapFlag& _flag,
    const WeightingType& _weighting_type)
    : flag(_flag)
    , weighting_type(_weighting_type)
    , active(true)
    , visible(true)
    , paint_hardness(0)
    , decay_rate(0)
    , grid(_level, sf::Color::Transparent)
    , total_weight(0)
    , highest_weight(0)
    , highest_weight_index(0)
{
    weightings.assign(_level.getProduct(), 0);
}


bool Heatmap::isActive() const
{
    return active;
}


void Heatmap::setActive(const bool _active)
{
    active = _active;
}


bool Heatmap::isVisible() const
{
    return visible;
}


void Heatmap::setVisible(const bool _visible)
{
    visible = _visible;
}


void Heatmap::setPaintHardness(const float _hardness)
{
    paint_hardness = _hardness;
}


void Heatmap::setDecayRate(const float _decay_rate)
{
    decay_rate = _decay_rate;
}


void Heatmap::paint(const int _tile_index, const int _radius)
{
    paintWithModifier(_tile_index, _radius, JTime::getUnscaledDeltaTime());
}


void Heatmap::splash(const int _tile_index, const int _radius)
{
    paintWithModifier(_tile_index, _radius, 1);
}


void Heatmap::setColor(const sf::Color& _color)
{
    color = _color;

    // Reset weightings.
    for (unsigned int i = 0; i < weightings.size(); ++i)
    {
        grid.setTileColor(i, _color);
        grid.setTileAlpha(i, 0);

        weightings[i] = MIN_WEIGHTING;
    }
}


void Heatmap::tick()
{
    if (decay_rate > 0)
        decay();
}


void Heatmap::draw(sf::RenderWindow& _window)
{
    grid.draw(_window);
}


int Heatmap::getWeight(const int _tile_index)
{
    if (!JHelper::validIndex(_tile_index, weightings.size()))
        return 0;

    return static_cast<int>(weightings[_tile_index]);
}


int Heatmap::getTotalWeight()
{
    return static_cast<int>(total_weight);
}


HeatmapFlag Heatmap::getFlag() const
{
    return flag;
}


void Heatmap::setWeightingType(const WeightingType& _weighting_type)
{
    weighting_type = _weighting_type;
}


WeightingType Heatmap::getWeightingType() const
{
    return weighting_type;
}


int Heatmap::getHighestWeightIndex() const
{
    return highest_weight_index;
}


void Heatmap::paintWithModifier(const int _tile_index, const int _radius,
    const float _modifier)
{
    int size_x = grid.getSizeX();
    int size_y = grid.getSizeY();

    sf::Vector2i coords = JHelper::calculateCoords(_tile_index, size_x);

    // Paint everything including and around coords.
    for (int row = coords.y - _radius; row <= coords.y + _radius; ++row)
    {
        for (int col = coords.x - _radius; col <= coords.x + _radius; ++col)
        {
            // Stop paint from flowing outside the displayed grid.
            if ((col < 0 || col >= size_x) || (row < 0 || row >= size_y))
                continue;

            int curr = JHelper::calculateIndex(col, row, size_x);
            int diff = JHelper::manhattanDistance({ col, row }, coords);

            auto& weighting = weightings[curr];

            if (weighting >= MAX_WEIGHTING)
                continue;

            // Manual clamp to accurately record change to total_weight.
            float adjustment = (paint_hardness / (diff + 1)) * _modifier;
            if (weighting + adjustment > MAX_WEIGHTING)
            {
                adjustment = MAX_WEIGHTING - weighting;
            }

            weighting += adjustment;
            total_weight += adjustment;

            updateHighestWeighting(weighting, curr);

            grid.setTileAlpha(curr, weighting);
        }
    }
}


/* Decays the entire heatmap based on its decay rate.
 * Also updates the heatmap's total_weight.
 */
void Heatmap::decay()
{
    total_weight = 0;
    highest_weight = 0;

    for (unsigned int i = 0; i < weightings.size(); ++i)
    {
        float& weighting = weightings[i];

        if (weighting == 0)
            continue;

        weighting -= (decay_rate * JTime::getDeltaTime());
        weighting = JMath::clampf(weighting, MIN_WEIGHTING, MAX_WEIGHTING);

        updateHighestWeighting(weighting, i);

        total_weight += weighting;

        grid.setTileAlpha(i, weighting);
    }
}


void Heatmap::updateHighestWeighting(const float _weighting, const int _index)
{
    if (_weighting > highest_weight)
    {
        highest_weight = _weighting;
        highest_weight_index = _index;
    }
}
