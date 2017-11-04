#include "NavNode.h"


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
