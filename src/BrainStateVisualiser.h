#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

namespace sf
{
    class RenderWindow;
}

struct GameData;

class BrainStateVisualiser
{
public:
    BrainStateVisualiser(GameData& _gd);
    ~BrainStateVisualiser() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void stateChanged(int _state_key);

private:
    void init();
    void initOverlays();
    void initSprite();

    void handleSlide();
    void slideOverlay(sf::RectangleShape* _overlay, const float _target_x);

    GameData& gd;

    sf::RectangleShape angry_overlay;
    sf::RectangleShape exhausted_overlay;

    sf::RectangleShape* sliding_in_overlay;
    sf::RectangleShape* sliding_out_overlay;

    float overlay_x_offset;
    float half_overlay_x_offset;
    float overlay_slide_speed;

};
