#include <algorithm>
#include <queue>
#include <limits>
#include <set>
#include <list>

#include "NavManager.h"
#include "JHelper.h"
#include "Level.h"
#include "Constants.h"


NavManager::NavManager(HeatmapManager& _heatmap_manager, const Level& _level)
    : heatmap_manager(_heatmap_manager)
    , size_x(_level.getSizeX())
    , size_y(_level.getSizeY())
{
    nav_nodes.assign(_level.getProduct(), NavNode());

    int index = 0;
    for (auto& node : nav_nodes)
    {
        node.setIndex(index);
        node.setCoords(JHelper::calculateCoords(index, size_x));

        evaluateNodeNeighbours(node, false);

        ++index;
    }
}


bool NavManager::isNodeWalkable(const int _index) const
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return false;

    return nav_nodes[_index].isWalkable();
}


void NavManager::setNodeWalkable(const int _index, const bool _walkable)
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return;

    auto& nav_node = nav_nodes[_index];
    nav_node.setWalkable(_walkable);
}


void NavManager::toggleNodeWalkable(const int _index)
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return;

    auto& nav_node = nav_nodes[_index];
    setNodeWalkable(_index, !nav_node.isWalkable());
}


NavPath NavManager::findPath(const sf::Vector2i& _start, const sf::Vector2i& _goal)
{
    NavPath path;

    int start_index = JHelper::calculateIndex(_start, size_x);
    int goal_index = JHelper::calculateIndex(_goal, size_x);

    if (!JHelper::validIndex(start_index, nav_nodes.size()) ||
        !JHelper::validIndex(goal_index, nav_nodes.size()))
    {
        return path;
    }

    resetGraph(); // This may not be needed ..

    NavNode* start_node = &nav_nodes[start_index];
    NavNode* goal_node = &nav_nodes[goal_index];

    start_node->setGCost(0);
    start_node->setHCost(JHelper::chebyshevDistance(_start, _goal));

    std::vector<NavNode*> open_list;
    std::vector<NavNode*> closed_list;

    open_list.push_back(start_node);

    while (!open_list.empty())
    {
        NavNode* curr = open_list[0];
        for (NavNode* node : open_list)
        {
            if (node->smallerFCost(*curr))
                curr = node;
        }

        open_list.erase(std::remove_if(
            open_list.begin(),
            open_list.end(),
            [curr](NavNode* _node) { return _node == curr; }),
            open_list.end());

        closed_list.push_back(curr);

        if (curr == goal_node)
        {
            path = retracePath(start_node, goal_node);

            break;
        }
        
        auto& neighbours = curr->getNeighbours();
        for (auto* neighbour : neighbours)
        {
            if (!neighbour->isWalkable())
                continue;

            int tentative_g = curr->getGCost() + JHelper::chebyshevDistance(curr->getCoords(), neighbour->getCoords());
            bool closed_contains_neighbour = std::find(closed_list.begin(), closed_list.end(), neighbour) != closed_list.end();

            if (closed_contains_neighbour && tentative_g >= neighbour->getGCost())
                continue;

            bool open_contains_neighbour = std::find(closed_list.begin(), closed_list.end(), neighbour) != closed_list.end();
            if (!open_contains_neighbour || tentative_g < neighbour->getGCost())
            {
                neighbour->setGCost(tentative_g);
                neighbour->setHCost(JHelper::chebyshevDistance(neighbour->getCoords(), _goal));
                neighbour->setParent(curr);

                if (!open_contains_neighbour)
                    open_list.push_back(neighbour);
            }
        }
    }

    return path;
}


void NavManager::evaluateNodeNeighbours(NavNode& _node, const bool _diagonals)
{
    std::vector<NavNode*> neighbours;
    neighbours.reserve(8);

    auto& coords = _node.getCoords();

    for (int row = coords.y - 1; row <= coords.y + 1; ++row)
    {
        for (int col = coords.x - 1; col <= coords.x + 1; ++col)
        {
            if ((col < 0 || col >= size_x) ||
                (row < 0 || row >= size_y) ||
                (col == coords.x && row == coords.y))
            {
                continue;
            }

            int diff = JHelper::manhattanDistance({ col, row }, coords);
            if (!_diagonals && diff == 2)
                continue;

            int index = JHelper::calculateIndex(col, row, size_x);
            neighbours.push_back(&nav_nodes[index]);
        }
    }

    _node.setNeighbours(neighbours);
}


void NavManager::resetGraph()
{
    const int INF = std::numeric_limits<int>::infinity();

    for (auto& node : nav_nodes)
    {
        node.setGCost(INF);
        node.setHCost(INF);
        node.setParent(nullptr);
    }
}


NavPath NavManager::retracePath(NavNode* _start_node, NavNode* _goal_node)
{
    NavPath path;
    path.success = true;

    NavNode* curr = _goal_node;
    while (curr != _start_node)
    {
        path.indices.push_back(curr->getIndex());
        curr = curr->getParent();
    }

    std::reverse(path.indices.begin(), path.indices.end());
    return path;
}
