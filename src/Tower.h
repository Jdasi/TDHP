#pragma once

#include <SFML/Graphics.hpp>

class Enemy;

class Tower
{
public:
    Tower(const int _tile_index);
    ~Tower() = default;

    void draw(sf::RenderWindow& _window);

    int getTileIndex() const;

    bool canShoot() const;
    void shoot(Enemy* _enemy);

    void setTexture(sf::Texture* _texture);
    
    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _position);

    void setScale(const sf::Vector2f& _factors);
    void setScale(const float _x, const float _y);

private:
    void initEngageRadiusDisplay();

    int tile_index;
    float last_shot_timestamp;

    sf::Sprite tower_sprite;
    sf::CircleShape engage_radius_display;

};
