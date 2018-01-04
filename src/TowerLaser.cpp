#include <SFML/Graphics/RenderWindow.hpp>

#include "TowerLaser.h"
#include "JTime.h"


TowerLaser::TowerLaser()
    : line(sf::LineStrip, 2)
    , visible_duration(0.1f)
    , draw_until_time(0)
{
    setColor(sf::Color::White);
}


void TowerLaser::draw(sf::RenderWindow& _window)
{
    _window.draw(line);
}


void TowerLaser::setColor(const sf::Color& _color)
{
    line[0].color = _color;
    line[1].color = _color;
}


void TowerLaser::onRefresh(const sf::Vector2f& _from, const sf::Vector2f& _to)
{
    line[0].position = _from;
    line[1].position = _to;
}
