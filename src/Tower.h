#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "JTime.h"

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
    struct TowerLaser
    {
        sf::VertexArray line;
        sf::Color line_color;
        float visible_duration;
        float draw_until_time;

        TowerLaser()
            : line(sf::LineStrip, 2)
            , line_color(sf::Color::Cyan)
            , visible_duration(0.1f)
            , draw_until_time(0)
        {
            line[0].color = line_color;
            line[1].color = line_color;
        }

        void draw(sf::RenderWindow& _window)
        {
            if (JTime::getTime() > draw_until_time)
                return;

            _window.draw(line);
        }

        void refresh(const sf::Vector2f& _a, const sf::Vector2f& _b)
        {
            line[0].position = _a;
            line[1].position = _b;

            draw_until_time = JTime::getTime() + visible_duration;
        }
    };

    void initEngageRadiusDisplay();

    int tile_index;
    float last_shot_timestamp;

    sf::Sprite tower_sprite;
    sf::CircleShape engage_radius_display;
    TowerLaser laser;

};
