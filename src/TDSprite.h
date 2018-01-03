#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderWindow;
}

class Level;
struct GameData;

class TDSprite
{
public:
    TDSprite();
    virtual ~TDSprite() = default;

    virtual void tick(GameData& _gd);
    virtual void draw(sf::RenderWindow& _window);

    int getTileIndex() const;
    void setTileIndex(const int _tile_index);

    void setTexture(sf::Texture* _texture);

    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _position);

    void setRotation(const float _angle);

    static void init(const Level& _level);

protected:
    virtual void onSetPosition() {}

    float getTileWidth();
    float getTileHeight();

private:
    int tile_index;
    sf::Sprite sprite;

    static float tile_width;
    static float tile_height;

};
