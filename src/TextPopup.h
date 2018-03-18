#pragma once

#include <SFML/Graphics/Text.hpp>

namespace sf
{
    class RenderWindow;
}

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class to represent a flickering text visualisation.
Owners of this class are responsible for cleanup when an
instance has expired.

The text visualisation is automatically center aligned.

---------------------------------------------------------*/
class TextPopup final
{
public:
    TextPopup();
    ~TextPopup() = default;

    void init(const sf::Font& _font, const int _char_size, const std::string& _str,
        const sf::Color& _fill_color, const sf::Color& _outline_color,
        float _duration, const sf::Vector2f& _float_speed, const sf::Vector2f& _pos);

    bool expired() const;

    void tick();
    void draw(sf::RenderWindow& _window);

private:
    sf::Text text;
    float expiry_time;
    sf::Vector2f float_speed;

    bool visible;

};
