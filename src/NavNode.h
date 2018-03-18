#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018



---------------------------------------------------------*/
class NavNode
{
public:
    NavNode();
    ~NavNode() = default;

    int getIndex() const;
    void setIndex(const int _index);

    const sf::Vector2i& getCoords() const;
    void setCoords(const sf::Vector2i& _coords);
    void setCoords(const int _x, const int _y);

    bool isWalkable() const;
    void setWalkable(const bool _walkable);
    void toggleWalkable();

    int getGCost() const;
    void setGCost(const int _g);

    int getHCost() const;
    void setHCost(const int _h);

    int getFCost() const;
    bool smallerFCost(const NavNode& _n) const
    {
        return getFCost() < _n.getFCost();
    }

    NavNode* getParent() const;
    void setParent(NavNode* _parent);

    const std::vector<NavNode*>& getNeighbours() const;
    void setNeighbours(const std::vector<NavNode*>& _neighbours);
    
private:
    bool walkable;
    int index;
    sf::Vector2i grid_coords;
    
    int g_cost; // Distance from the start node.
    int h_cost; // Distance from the end node. The heuristic.

    NavNode* parent;
    std::vector<NavNode*> neighbours;

};
