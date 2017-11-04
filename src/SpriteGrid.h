#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class SpriteGrid
{
public:
    SpriteGrid(const int _size_x, const int _size_y, const float _padding,
        const sf::Color& _initial_color);

    ~SpriteGrid() = default;

    void draw(sf::RenderWindow& _window);

private:
    void init(const sf::Color& _initial_color);

    int size_x;
    int size_y;
    float padding;

    std::vector<sf::RectangleShape> sprites;

};
