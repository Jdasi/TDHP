#include <SFML/Graphics/RenderWindow.hpp>

#include "TDSprite.h"
#include "JHelper.h"


TDSprite::TDSprite()
    : tile_index(0)
{
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
    sprite.setTexture(*_texture);
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


void TDSprite::setScale(const sf::Vector2f& _factors)
{
    sprite.setScale(_factors);
    JHelper::centerSFOrigin(sprite);
}


void TDSprite::setScale(const float _x, const float _y)
{
    setScale({ _x, _y });
}
