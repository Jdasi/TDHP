#pragma once

#include <vector>
#include <memory>

#include "NavNode.h"
#include "HeatMap.h"
#include "TileGrid.h"

struct Level;

class NavManager
{
public:
    NavManager();
    ~NavManager() = default;

    void parseLevel(const Level& _level);
    HeatMap* createHeatMap(const sf::Color& _color,
        const float _paint_hardness, const float _decay_rate);

    void toggleTileWalkable(const sf::Vector2f& _pos);
    void toggleTileWalkable(const int _index);
    void paintOnHeatMap(const int _heatmap_index, const sf::Vector2f& _pos, const int _radius);

    void tick();
    void drawBaseLayer(sf::RenderWindow& _window);
    void drawHeatMaps(sf::RenderWindow& _window);

private:
    std::vector<NavNode> nav_nodes; // Base layer.
    std::vector<std::unique_ptr<HeatMap>> heat_maps;

    sf::RectangleShape border;
    std::unique_ptr<TileGrid> grid;

};
