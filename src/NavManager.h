#pragma once

#include <vector>

#include "NavNode.h"

class HeatmapManager;

class NavManager
{
public:
    NavManager(HeatmapManager& _heatmap_manager, const int _level_width,
        const int _level_height);

    ~NavManager() = default;

    bool isTileWalkable(const int _index) const;
    void toggleTileWalkable(const int _index);

private:
    std::vector<NavNode> nav_nodes;
    HeatmapManager& heatmap_manager;

};
