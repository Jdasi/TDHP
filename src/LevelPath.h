#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "NavPath.h"
#include "Waypoint.h"
#include "Level.h"
#include "JHelper.h"

class LevelPath
{
public:
    LevelPath();
    LevelPath(const Level& _level, const NavPath& _path);

    void draw(sf::RenderWindow& _window, const sf::Vector2f& _from,
        const int _from_index);

    bool pathSuccessful() const;
    int getTotalCost() const;

    int getNumPoints() const;
    const Waypoint& getWaypoint(const int _index) const;

private:
    NavPath raw_path;

    int num_points;
    std::vector<Waypoint> waypoints;

};
