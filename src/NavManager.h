#pragma once

#include <vector>

#include "NavNode.h"

class HeatmapManager;
class Level;

class NavManager
{
public:
    NavManager(HeatmapManager& _heatmap_manager, Level& _level);
    ~NavManager() = default;

    bool isNodeWalkable(const int _index) const;
    void setNodeWalkable(const int _index, const bool _walkable);
    void toggleNodeWalkable(const int _index);

private:
    std::vector<NavNode> nav_nodes;
    HeatmapManager& heatmap_manager;
    Level& level;

};
