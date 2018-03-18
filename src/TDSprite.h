#pragma once

#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    class RenderWindow;
}

class Level;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Tower Defence sprite class. A base class for all entities
to use that require a size-appropriate visualisation
for the current loaded level.

The static init() function should be called once during
game initialisation to allow for all TDSprites to correctly
size themselves.

The enclosed virtual functions act as events for derived classes,
which hides base behaviour and allows it to be extended.

---------------------------------------------------------*/
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
