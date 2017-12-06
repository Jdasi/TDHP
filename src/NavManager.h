#pragma once

#include <vector>

#include "NavNode.h"

class HeatmapManager;
class Level;

class NavManager
{
public:
    NavManager(HeatmapManager& _heatmap_manager, const Level& _level);
    ~NavManager() = default;

    bool isTileWalkable(const int _index) const;
    void toggleTileWalkable(const int _index);

private:
    std::vector<NavNode> nav_nodes;
    HeatmapManager& heatmap_manager;

};
