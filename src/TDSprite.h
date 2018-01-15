#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderWindow;
}

class Level;

class TDSprite
{
public:
    TDSprite();
    virtual ~TDSprite() = default;

    virtual void tick();
    virtual void draw(sf::RenderWindow& _window);

    int getTileIndex() const;
    void setTileIndex(const int _tile_index);

    void setTexture(sf::Texture* _texture);
    void setColor(const sf::Color& _color);

    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _position);

    void setRotation(const float _angle);

    bool collisionCheck(const sf::Vector2f& _point);

    static void init(Level& _level);

protected:
    virtual void onSetPosition() {}

    int getLevelSizeX() const;
    int getLevelSizeY() const;

    float getLevelTileWidth() const;
    float getLevelTileHeight() const;

private:
    int tile_index;
    sf::Sprite sprite;

    static Level* current_level;

};
