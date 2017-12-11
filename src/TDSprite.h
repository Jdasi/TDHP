#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderWindow;
}

class TDSprite
{
public:
    TDSprite();
    virtual ~TDSprite() = default;

    virtual void draw(sf::RenderWindow& _window);

    int getTileIndex() const;
    void setTileIndex(const int _tile_index);

    void setTexture(sf::Texture* _texture);

    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _position);

    void setScale(const sf::Vector2f& _factors);
    void setScale(const float _x, const float _y);

protected:
    virtual void onSetPosition() {}

private:
    int tile_index;
    sf::Sprite sprite;

};
