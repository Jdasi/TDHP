#include "LevelPath.h"

LevelPath::LevelPath()
    : num_points(0)
{
}


LevelPath::LevelPath(const Level& _level, const NavPath& _path)
    : num_points(_path.indices.size())
{
    raw_path = _path;
    waypoints.reserve(num_points);

    for (auto& index : _path.indices)
    {
        auto coords = JHelper::calculateCoords(index, _level.getSizeX());
        auto pos = JHelper::tileIndexToPos(index, _level);

        waypoints.push_back({index, coords, pos});
    }
}


void LevelPath::draw(sf::RenderWindow& _window, const sf::Vector2f& _from,
    const unsigned int _from_index, const sf::Color& _color)
{
    if (_from_index >= waypoints.size())
        return;

    // Cause the line visualisation to shrink based on _from_index.
    int line_size = num_points - _from_index + 1;
    sf::VertexArray vis(sf::PrimitiveType::LineStrip, line_size);

    for (int i = 0; i < line_size; ++i)
    {
        vis[i].color = _color;
        vis[i].position = i == 0 ? _from : waypoints[_from_index + i - 1].pos;
    }

    _window.draw(vis);
}


bool LevelPath::pathSuccessful() const
{
    return raw_path.success;
}


int LevelPath::getTotalCost() const
{
    return raw_path.total_cost;
}


int LevelPath::getNumPoints() const
{
    return waypoints.size();
}


const Waypoint& LevelPath::getWaypoint(const int _index) const
{
    return waypoints[_index];
}
