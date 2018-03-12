#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
    struct AnimSettings
    {
        void horizontalSway(const float _spd, const float _str)
        {
            h_speed = _spd;
            h_strength = _str;
        }

        void verticalSway(const float _spd, const float _str)
        {
            v_speed = _spd;
            v_strength = _str;
        }

        void rotation(const float _delay, const float _str)
        {
            r_delay = _delay;
            r_strength = _str;
        }

        float h_speed = 0;
        float h_strength = 0;

        float v_speed = 0;
        float v_strength = 0;

        float r_delay = 0;
        float r_strength = 0;
    };

    void init();
    void initOverlays();
    void initSprite();

    void handleSlide();
    void handleSpriteAnim();

    void slideOverlay(sf::RectangleShape* _overlay, const float _target_x);

    GameData& gd;

    sf::RectangleShape angry_overlay;
    sf::RectangleShape exhausted_overlay;

    sf::RectangleShape* sliding_in_overlay;
    sf::RectangleShape* sliding_out_overlay;

    float overlay_x_offset;
    float half_overlay_x_offset;
    float overlay_slide_speed;

    sf::Vector2f sprite_origin;
    sf::Sprite sprite;
    AnimSettings anim_settings;

    float rot_timer;

};
