#pragma once

#include "TDSprite.h"
#include "Projectile.h"

class TowerBullet final : public Projectile
{
public:
    TowerBullet();

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void setTexture(sf::Texture* _texture);
    void setColor(const sf::Color& _color) override;

    const sf::Vector2f& getPosition() const;

private:
    void onRefresh(const sf::Vector2f& _from, const sf::Vector2f& _to) override;

    TDSprite sprite;
    sf::Vector2f direction;

    float visible_duration;
    float draw_until_time;

};