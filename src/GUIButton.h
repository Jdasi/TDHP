#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include <functional>
#include <vector>

namespace sf
{
    class RenderWindow;
}

struct GameData;

class GUIButton
{
public:
    GUIButton();
    GUIButton(const sf::Color& _normal, const sf::Color& _hover, const sf::Color& _press);
    ~GUIButton() = default;

    void tick(GameData& _gd);
    void draw(sf::RenderWindow& _window);

    void setTexture(sf::Texture* _texture);
    void setColors(const sf::Color& _normal, const sf::Color& _hover, const sf::Color& _press);

    const sf::Vector2f& getPosition() const;
    void setPosition(const sf::Vector2f& _pos);
    void setPosition(const float _x, const float _y);

    const sf::Vector2f& getSize() const;
    void setSize(const sf::Vector2f& _size);

    const sf::Vector2f& getScale() const;
    void setScale(const sf::Vector2f& _factors);

    float getRotation() const;
    void setRotation(const float _angle);

    void addClickEvent(const std::function<void()>& _function);
    void clearClickEvents();

private:
    void handleClick(GameData& _gd);
    void handleColor();
    
    void onClick();

    sf::Color normal_color;
    sf::Color hover_color;
    sf::Color pressed_color;

    sf::RectangleShape shape;
    bool hover;
    bool pressed;

    std::vector<std::function<void()>> click_events;

};
