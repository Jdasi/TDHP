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

    HeatMap* createHeatMap(const sf::Color& _color,
        const float _paint_hardness, const float _decay_rate);

    void toggleTileWalkable(const sf::Vector2f& _pos);
    void paintOnHeatMap(const int _heatmap_index, const sf::Vector2f& _pos, const int _radius);

    void tick();
    void drawBaseLayer(sf::RenderWindow& _window);
    void drawHeatMaps(sf::RenderWindow& _window);

private:
    std::vector<NavNode> nav_nodes; // Base layer.
    std::vector<std::unique_ptr<HeatMap>> heat_maps;

    sf::RectangleShape border;
    TileGrid grid;

};
