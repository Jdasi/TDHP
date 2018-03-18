#pragma once

#include <SFML/Graphics/VertexArray.hpp>

#include "Projectile.h"

namespace sf
{
    class RenderWindow;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

A Projectile that visualises a line between a start and end point.

---------------------------------------------------------*/
class TowerLaser final : public Projectile
{
public:
    TowerLaser();

    void draw(sf::RenderWindow& _window) override;

    void setColor(const sf::Color& _color) override;

private:
    void onRefresh(const sf::Vector2f& _from, const sf::Vector2f& _to) override;

    sf::VertexArray line;
    sf::Color line_color;

    float visible_duration;
    float draw_until_time;

};
