#include <SFML/Graphics/RenderWindow.hpp>

#include "TextPopup.h"
#include "JTime.h"
#include "JHelper.h"


TextPopup::TextPopup()
    : expiry_time(0)
    , visible(true)
{
}


void TextPopup::init(const sf::Font& _font, const int _char_size, const std::string& _str,
    const sf::Color& _fill_color, const sf::Color& _outline_color,
    float _duration, const sf::Vector2f& _float_speed, const sf::Vector2f& _pos)
{
    expiry_time = JTime::getTime() + _duration;

    text.setFont(_font);
    text.setCharacterSize(_char_size);
    text.setFillColor(_fill_color);
    text.setOutlineColor(_outline_color);
    text.setString(_str);
    text.setPosition(_pos);

    float_speed = _float_speed;
    text.setOutlineThickness(1);

    JHelper::centerSFOrigin(text);
}


bool TextPopup::expired() const
{
    return JTime::getTime() >= expiry_time;
}


void TextPopup::tick()
{
    text.setPosition(text.getPosition() + (float_speed * JTime::getDeltaTime()));
}


void TextPopup::draw(sf::RenderWindow& _window)
{
    if (visible)
    {
        _window.draw(text);
    }

    visible = !visible;
}
