#pragma once

#include <vector>
#include <memory>

#include "NavNode.h"
#include "TileGrid.h"

class HeatmapManager;
struct Level;

class NavManager
{
public:
    NavManager(HeatmapManager* _heatmap_manager);
    ~NavManager() = default;

    void parseLevel(const Level& _level);
    void draw(sf::RenderWindow& _window);

    bool isTileWalkable(const int _index) const;
    void toggleTileWalkable(const sf::Vector2f& _pos);
    void toggleTileWalkable(const int _index);

    sf::Vector2f getTileWorldPos(const int _tile_index);

private:
    std::vector<NavNode> nav_nodes;
    std::unique_ptr<TileGrid> grid;

    HeatmapManager* heatmap_manager;

};
