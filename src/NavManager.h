#pragma once

#include "NavNode.h"
#include "HeatMap.h"
#include "SpriteGrid.h"

#include <vector>

class NavManager
{
public:
    NavManager();
    ~NavManager() = default;

    void colorTileAtPos(const sf::Vector2f& _pos);

    void tick();
    void draw(sf::RenderWindow& _window);

private:
    std::vector<NavNode> nav_nodes;
    std::vector<HeatMap> heat_maps;

    sf::RectangleShape border;
    SpriteGrid grid;

};
