#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "NavPath.h"
#include "Waypoint.h"
#include "Level.h"
#include "JHelper.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class which represents a navigation path that corresponds with
the positions of a level's tiles, allowing for other visualisations
to appear correctly within the level.

---------------------------------------------------------*/
class LevelPath final
{
public:
    LevelPath();
    LevelPath(const Level& _level, const NavPath& _path);

    void draw(sf::RenderWindow& _window, const sf::Vector2f& _from,
        const unsigned int _from_index = 0, const sf::Color& _color = sf::Color::Yellow);

    bool pathSuccessful() const;
    int getTotalCost() const;

    int getNumPoints() const;
    const Waypoint& getWaypoint(const int _index) const;
    const Waypoint& getLastWaypoint() const;

private:
    NavPath raw_path;

    int num_points;
    std::vector<Waypoint> waypoints;

};
