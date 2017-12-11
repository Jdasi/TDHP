#include "LevelPath.h"

LevelPath::LevelPath()
    : num_points(0)
{
}


LevelPath::LevelPath(const Level& _level, const NavPath& _path)
    : num_points(_path.indices.size())
    , vis(sf::PrimitiveType::LineStrip, num_points)
{
    waypoints.reserve(num_points);

    for (auto& index : _path.indices)
    {
        auto coords = JHelper::calculateCoords(index, _level.getSizeX());
        auto pos = JHelper::tileIndexToPos(index, _level);

        waypoints.push_back({index, coords, pos});
    }

    for (int i = 0; i < num_points; ++i)
    {
        vis[i].color = sf::Color::Yellow;
        vis[i].position = waypoints[i].pos;
    }
}


void LevelPath::draw(sf::RenderWindow& _window)
{
    _window.draw(vis);
}


int LevelPath::getNumPoints() const
{
    return num_points;
}


const Waypoint& LevelPath::getWaypoint(const int _index) const
{
    return waypoints[_index];
}
