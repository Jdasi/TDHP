#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "TDSprite.h"
#include "JHelper.h"
#include "Level.h"


Level* TDSprite::current_level = nullptr;


TDSprite::TDSprite()
    : tile_index(0)
{
}


void TDSprite::tick()
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
    sprite.setScale(current_level->getTileWidth() / texture_size.x,
        current_level->getTileHeight() / texture_size.y);

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


void TDSprite::init(Level& _level)
{
    current_level = &_level;
}


int TDSprite::getLevelSizeX() const
{
    if (current_level == nullptr)
        return 0;

    return current_level->getSizeX();
}


int TDSprite::getLevelSizeY() const
{
    if (current_level == nullptr)
        return 0;

    return current_level->getSizeY();
}


float TDSprite::getLevelTileWidth() const
{
    if (current_level == nullptr)
        return 0;

    return current_level->getTileWidth();
}


float TDSprite::getLevelTileHeight() const
{
    if (current_level == nullptr)
        return 0;

    return current_level->getTileHeight();
}
