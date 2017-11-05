#pragma once

#include "NavNode.h"
#include "HeatMap.h"
#include "TileGrid.h"

#include <vector>
#include <memory>

class NavManager
{
public:
    NavManager();
    ~NavManager() = default;

    HeatMap* createHeatMap(const sf::Color& _hot_color, const sf::Color& _cold_color,
        const float _paint_hardness, const float _decay_rate);

    void toggleTileWalkable(const sf::Vector2f& _pos);

    void tick();
    void draw(sf::RenderWindow& _window);

private:
    std::vector<NavNode> nav_nodes;
    std::vector<std::unique_ptr<HeatMap>> heat_maps;

    sf::RectangleShape border;
    TileGrid grid;

};
