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

    bool isTileWalkable(const int _index) const;
    void toggleTileWalkable(const sf::Vector2f& _pos);
    void toggleTileWalkable(const int _index);

    void paintOnHeatMap(const int _heatmap_index, const int _tile_index, const int _radius);
    void splashOnHeatMap(const int _heatmap_index, const int _tile_index, const int _radius);

    sf::Vector2f getTileWorldPos(const int _tile_index);

    void tick();
    void drawBaseLayer(sf::RenderWindow& _window);
    void drawHeatMaps(sf::RenderWindow& _window);

private:
    std::vector<NavNode> nav_nodes; // Base layer.
    std::unique_ptr<TileGrid> grid;

    std::vector<std::unique_ptr<HeatMap>> heat_maps;

};
