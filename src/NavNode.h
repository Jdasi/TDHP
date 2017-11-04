#pragma once

#include <SFML/System/Vector2.hpp>

class NavNode
{
public:
    NavNode() = default;
    ~NavNode() = default;

    const sf::Vector2i& getCoords() const;
    void setCoords(const sf::Vector2i& _coords);
    void setCoords(const int _x, const int _y);

    bool isWalkable() const;
    void setWalkable(const bool _walkable);

    int getGCost() const;
    void setGCost(const int _g);

    int getHCost() const;
    void setHCost(const int _h);

    int getFCost() const;

    NavNode* getParent() const;
    void setParent(NavNode* _parent);

private:
    bool walkable;
    sf::Vector2i grid_coords;
    
    int g_cost; // Distance from the start node.
    int h_cost; // Distance from the end node.

    NavNode* parent;

};
