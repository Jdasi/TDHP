#include "NavNode.h"


NavNode::NavNode()
    : walkable(true)
    , g_cost(0)
    , h_cost(0)
    , parent(nullptr)
{
    neighbours.reserve(8);
}


int NavNode::getIndex() const
{
    return index;
}

void NavNode::setIndex(const int _index)
{
    index = _index;
}


const sf::Vector2i& NavNode::getCoords() const
{
    return grid_coords;
}


void NavNode::setCoords(const sf::Vector2i& _coords)
{
    grid_coords = _coords;
}


void NavNode::setCoords(const int _x, const int _y)
{
    setCoords(sf::Vector2i(_x, _y));
}


bool NavNode::isWalkable() const
{
    return walkable;
}


void NavNode::setWalkable(const bool _walkable)
{
    walkable = _walkable;
}


void NavNode::toggleWalkable()
{
    setWalkable(!isWalkable());
}


int NavNode::getGCost() const
{
    return g_cost;
}


void NavNode::setGCost(const int _g)
{
    g_cost = _g;
}


int NavNode::getHCost() const
{
    return h_cost;
}


void NavNode::setHCost(const int _h)
{
    h_cost = _h;
}


int NavNode::getFCost() const
{
    return g_cost + h_cost;
}


NavNode* NavNode::getParent() const
{
    return parent;
}


void NavNode::setParent(NavNode* _parent)
{
    parent = _parent;
}


const std::vector<NavNode*>& NavNode::getNeighbours() const
{
    return neighbours;
}


void NavNode::setNeighbours(const std::vector<NavNode*>& _neighbours)
{
    neighbours = _neighbours;
}
