#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "TDSprite.h"
#include "JHelper.h"
#include "Level.h"


float TDSprite::tile_width = 0;
float TDSprite::tile_height = 0;


TDSprite::TDSprite()
    : tile_index(0)
{
}


void TDSprite::tick(GameData& _gd)
{
    // Base class doesn't do anything.
}


void TDSprite::draw(sf::RenderWindow& _window)
{
    _window.draw(sprite);
}


int TDSprite::getTileIndex() const
{
    return tile_index;
}


void TDSprite::setTileIndex(const int _tile_index)
{
    tile_index = _tile_index;
}


void TDSprite::setTexture(sf::Texture* _texture)
{
    auto texture_size = _texture->getSize();

    sprite.setTexture(*_texture);
    sprite.setScale(tile_width / texture_size.x,
        tile_height / texture_size.y);

    JHelper::centerSFOrigin(sprite);
}


void TDSprite::setColor(const sf::Color& _color)
{
    sprite.setColor(_color);
}


const sf::Vector2f& TDSprite::getPosition() const
{
    return sprite.getPosition();
}


void TDSprite::setPosition(const sf::Vector2f& _position)
{
    sprite.setPosition(_position);
    onSetPosition();
}


void TDSprite::setRotation(const float _angle)
{
    sprite.setRotation(_angle);
}


bool TDSprite::collisionCheck(const sf::Vector2f& _point)
{
    return sprite.getGlobalBounds().contains(_point);
}


void TDSprite::init(const Level& _level)
{
    tile_width = _level.getTileWidth();
    tile_height = _level.getTileHeight();
}


float TDSprite::getTileWidth()
{
    return tile_width;
}


float TDSprite::getTileHeight()
{
    return tile_height;
}
