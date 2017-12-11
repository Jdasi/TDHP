#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#include "TDSprite.h"
#include "Killable.h"
#include "JTime.h"

class Enemy;

class Tower final : public TDSprite, public Killable
{
public:
    Tower();
    ~Tower() = default;

    void draw(sf::RenderWindow& _window) override;

    bool canShoot() const;
    void shoot(Enemy* _enemy);

protected:
    void onSpawn() override;
    void onDeath() override;

    void onSetPosition() override;

private:
    struct TowerLaser
    {
        TowerLaser();

        void draw(sf::RenderWindow& _window);
        void refresh(const sf::Vector2f& _from, const sf::Vector2f& _to);

        sf::VertexArray line;
        sf::Color line_color;
        float visible_duration;
        float draw_until_time;
    };

    void initEngageRadiusDisplay();

    sf::CircleShape engage_radius_display;
    TowerLaser laser;
    float last_shot_timestamp;

};
