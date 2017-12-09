#pragma once

#include <vector>

#include "NavNode.h"

class HeatmapManager;
class Level;

struct NavPath
{
    NavPath()
        : success(false)
        , total_cost(0)
    {
    }

    std::vector<int> indices;
    bool success;
    int total_cost;
};

class NavManager
{
public:
    NavManager(HeatmapManager& _heatmap_manager, const Level& _level);
    ~NavManager() = default;

    bool isNodeWalkable(const int _index) const;
    void setNodeWalkable(const int _index, const bool _walkable);
    void toggleNodeWalkable(const int _index);

    NavPath findPath(const sf::Vector2i& _start, const sf::Vector2i& _goal);

private:
    void evaluateNodeNeighbours(NavNode& _node, const bool _diagonals);
    void resetGraph();
    NavPath retracePath(NavNode* _start_node, NavNode* _goal_node);

    std::vector<NavNode> nav_nodes;
    HeatmapManager& heatmap_manager;

    int size_x;
    int size_y;

};
