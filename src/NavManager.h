#pragma once

#include <vector>

#include "NavNode.h"
#include "NavPath.h"

class HeatmapManager;
class Level;

class NavManager
{
public:
    enum HeuristicType
    {
        MANHATTAN,
        CHEBYSHEV
    };

    NavManager(HeatmapManager& _heatmap_manager, const Level& _level);
    ~NavManager() = default;

    bool isNodeWalkable(const int _index) const;
    void setNodeWalkable(const int _index, const bool _walkable);
    void toggleNodeWalkable(const int _index);

    NavPath findPath(const sf::Vector2i& _start, const sf::Vector2i& _goal);

private:
    void evaluateNodeNeighbours(NavNode& _node);
    void resetGraph();
    int calculateHeuristic(const sf::Vector2i& _a, const sf::Vector2i& _b);
    NavPath retracePath(NavNode* _start_node, NavNode* _goal_node);

    std::vector<NavNode> nav_nodes;
    HeuristicType heuristic_type;

    HeatmapManager& heatmap_manager;

    int size_x;
    int size_y;

};
