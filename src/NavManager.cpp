#include <algorithm>

#include "NavManager.h"
#include "HeatmapManager.h"
#include "JHelper.h"
#include "JMath.h"
#include "Level.h"
#include "Constants.h"


NavManager::NavManager(HeatmapManager& _heatmap_manager, const Level& _level)
    : heatmap_manager(_heatmap_manager)
    , size_x(_level.getSizeX())
    , size_y(_level.getSizeY())
    , heuristic_type(HeuristicType::MANHATTAN)
{
    nav_nodes.assign(_level.getProduct(), NavNode());

    int index = 0;
    for (auto& node : nav_nodes)
    {
        node.setIndex(index);
        node.setCoords(JHelper::calculateCoords(index, size_x));

        evaluateNodeNeighbours(node);

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


NavPath NavManager::findPath(const sf::Vector2i& _start, const sf::Vector2i& _goal,
    const HeatmapFlag& _heatmap_flags)
{
    NavPath path;

    int start_index = JHelper::calculateIndex(_start, size_x);
    int goal_index = JHelper::calculateIndex(_goal, size_x);

    if (!JHelper::validIndex(start_index, nav_nodes.size()) ||
        !JHelper::validIndex(goal_index, nav_nodes.size()))
    {
        return path;
    }

    resetGraph();

    NavNode* start_node = &nav_nodes[start_index];
    NavNode* goal_node = &nav_nodes[goal_index];

    start_node->setGCost(0);
    start_node->setHCost(calculateHeuristic(_start, _goal));

    std::vector<NavNode*> open_list;
    std::vector<NavNode*> closed_list;

    open_list.push_back(start_node);

    processOpenList(_goal, _heatmap_flags, path, start_node, goal_node,
        open_list, closed_list);

    return path;
}


void NavManager::evaluateNodeNeighbours(NavNode& _node)
{
    std::vector<NavNode*> neighbours;
    neighbours.reserve(heuristic_type == HeuristicType::MANHATTAN ? 4 : 8);

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
            if (heuristic_type == HeuristicType::MANHATTAN && diff == 2)
                continue;

            int index = JHelper::calculateIndex(col, row, size_x);
            neighbours.push_back(&nav_nodes[index]);
        }
    }

    _node.setNeighbours(std::move(neighbours));
}


void NavManager::resetGraph()
{
    int max_int = JMath::maxInt();

    for (auto& node : nav_nodes)
    {
        node.setGCost(max_int);
        node.setHCost(max_int);
        node.setParent(nullptr);
    }
}


int NavManager::calculateHeuristic(const sf::Vector2i& _a, const sf::Vector2i& _b)
{
    int heuristic = 0;

    switch (heuristic_type)
    {
        case MANHATTAN: heuristic = JHelper::manhattanDistance(_a, _b); break;
        case CHEBYSHEV: heuristic = JHelper::chebyshevDistance(_a, _b); break;
    }

    // Modifier makes distance heuristic competitive against heatmap values (0-255).
    return heuristic * HEURISTIC_MODIFIER;
}


void NavManager::processOpenList(const sf::Vector2i& _goal, const HeatmapFlag& _heatmap_flags,
    NavPath& path, NavNode* start_node, NavNode* goal_node,
    std::vector<NavNode*>& open_list, std::vector<NavNode*>& closed_list)
{
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
            bool closed_contains_neighbour = listContainsNode(closed_list, neighbour);
            if ((!neighbour->isWalkable() || closed_contains_neighbour) && neighbour != goal_node)
                continue;

            // Consider distance from start AND heatmap weights.
            int heuristic = calculateHeuristic(curr->getCoords(), neighbour->getCoords());
            int heatmap_weight = heatmap_manager.getWeight(curr->getIndex(), _heatmap_flags);

            int tentative_g = curr->getGCost() + heuristic + heatmap_weight;

            bool open_contains_neighbour = listContainsNode(open_list, neighbour);
            if (!open_contains_neighbour || tentative_g < neighbour->getGCost())
            {
                neighbour->setGCost(tentative_g);
                neighbour->setHCost(calculateHeuristic(neighbour->getCoords(), _goal));
                neighbour->setParent(curr);

                if (!open_contains_neighbour)
                {
                    open_list.push_back(neighbour);
                }
            }
        }
    }
}


bool NavManager::listContainsNode(std::vector<NavNode*>& _list, NavNode* _node) const
{
    return std::find(_list.begin(), _list.end(), _node) != _list.end();
}


NavPath NavManager::retracePath(NavNode* _start_node, NavNode* _goal_node)
{
    NavPath path;
    path.success = true;

    NavNode* curr = _goal_node;
    sf::Vector2i last_coords = _goal_node->getCoords();

    while (curr != _start_node)
    {
        NavNode* next = curr->getParent();
        bool curr_is_corner = false;

        if (next != nullptr)
        {
            sf::Vector2i next_coords = next->getCoords();
            curr_is_corner = (next_coords.x != last_coords.x && next_coords.y != last_coords.y);
        }

        // Minimise the amount of straight-line nodes.
        if (curr == _goal_node || curr_is_corner)
        {
            path.indices.push_back(curr->getIndex());
            last_coords = curr->getCoords();
        }

        curr = curr->getParent();
    }

    // Path must be flipped as it was traced from goal to start.
    path.total_cost = _goal_node->getGCost();
    std::reverse(path.indices.begin(), path.indices.end());

    return path;
}
