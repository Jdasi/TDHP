#include <SFML/Graphics/RenderWindow.hpp>

#include "BrainStateVisualiser.h"
#include "BrainStateType.h"
#include "GameData.h"
#include "AssetManager.h"
#include "GameAudio.h"
#include "JHelper.h"
#include "JTime.h"
#include "Constants.h"


BrainStateVisualiser::BrainStateVisualiser(GameData& _gd)
    : gd(_gd)
    , overlay_x_offset(200)
    , half_overlay_x_offset(overlay_x_offset / 2)
    , overlay_slide_speed(1500)
    , rot_timer(0)
{
    init();
}


void BrainStateVisualiser::tick()
{
    handleSlide();
    handleSpriteAnim();
}


void BrainStateVisualiser::draw(sf::RenderWindow& _window)
{
    _window.draw(sprite);
    _window.draw(angry_overlay);
    _window.draw(exhausted_overlay);
}


void BrainStateVisualiser::stateChanged(int _state_key)
{
    switch (_state_key)
    {
        case BRAINSTATE_NORMAL:     onStateNormal();        break;
        case BRAINSTATE_ANGRY:      onStateAngry();         break;
        case BRAINSTATE_EXHAUSTED:  onStateExhausted();     break;
    }
}


void BrainStateVisualiser::init()
{
    initOverlays();
    initSprite();
}


void BrainStateVisualiser::initOverlays()
{
    auto* tex = gd.assets.loadTexture(BS_OVERLAY_TEXTURE);

    angry_overlay.setSize(WINDOW_SIZE + sf::Vector2f(overlay_x_offset, 0));
    angry_overlay.setFillColor(sf::Color(255, 0, 0, 50));
    angry_overlay.setTexture(tex);

    exhausted_overlay.setSize(WINDOW_SIZE + sf::Vector2f(overlay_x_offset, 0));
    exhausted_overlay.setFillColor(sf::Color(0, 0, 255, 50));
    exhausted_overlay.setTexture(tex);
}


void BrainStateVisualiser::initSprite()
{
    sprite_origin = sf::Vector2f(50, 60);

    sprite.setTexture(*gd.assets.loadTexture(E_BASIC_TEXTURE));
    sprite.setPosition(sprite_origin);

    JHelper::centerSFOrigin(sprite);
}


void BrainStateVisualiser::onStateNormal()
{
    angry_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);
    exhausted_overlay.setPosition(-overlay_x_offset, 0);

    sliding_in_overlay = nullptr;
    sliding_out_overlay = &exhausted_overlay;

    sprite.setTexture(*gd.assets.loadTexture(BS_NORMAL_TEXTURE));

    anim_settings.horizontalSway(0, 0);
    anim_settings.verticalSway(2.0f, 10.0f);
    anim_settings.rotation(0, 0);

    gd.audio.playSound(BS_NORMAL_SOUND);
}


void BrainStateVisualiser::onStateAngry()
{
    angry_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);
    exhausted_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);

    sliding_in_overlay = &angry_overlay;
    sliding_out_overlay = nullptr;

    sprite.setTexture(*gd.assets.loadTexture(BS_ANGRY_TEXTURE));

    anim_settings.horizontalSway(0, 0);
    anim_settings.verticalSway(15.0f, 5.0f);
    anim_settings.rotation(0.03f, 3);

    gd.audio.playSound(BS_ANGRY_SOUND);
}


void BrainStateVisualiser::onStateExhausted()
{
    angry_overlay.setPosition(-half_overlay_x_offset, 0);
    exhausted_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);

    sliding_in_overlay = &exhausted_overlay;
    sliding_out_overlay = &angry_overlay;

    sprite.setTexture(*gd.assets.loadTexture(BS_EXHAUSTED_TEXTURE));

    anim_settings.horizontalSway(0, 0);
    anim_settings.verticalSway(1.5f, 10.0f);
    anim_settings.rotation(0, 0);

    gd.audio.playSound(BS_EXHAUSTED_SOUND);
}


void BrainStateVisualiser::handleSlide()
{
    if (sliding_in_overlay != nullptr)
    {
        slideOverlay(sliding_in_overlay, -half_overlay_x_offset);
    }

    if (sliding_out_overlay != nullptr)
    {
        slideOverlay(sliding_out_overlay, WINDOW_WIDTH + overlay_x_offset);
    }
}


void BrainStateVisualiser::handleSpriteAnim()
{
    float h_sway = sin(JTime::getTime() * anim_settings.h_speed) * anim_settings.h_strength;
    float v_sway = sin(JTime::getTime() * anim_settings.v_speed) * anim_settings.v_strength;

    sprite.setPosition(sprite_origin + sf::Vector2f(h_sway, v_sway));

    rot_timer += JTime::getDeltaTime();
    if (rot_timer >= anim_settings.r_delay)
    {
        rot_timer = 0;

        float new_rot = sprite.getRotation() == anim_settings.r_strength ?
            -anim_settings.r_strength : anim_settings.r_strength;

        sprite.setRotation(new_rot);
    }
}


void BrainStateVisualiser::slideOverlay(sf::RectangleShape* _overlay, const float _target_x)
{
    auto& pos = _overlay->getPosition();
    if (pos.x < _target_x)
    {
        float adjustment = overlay_slide_speed * JTime::getDeltaTime();
        if (pos.x + adjustment > _target_x)
        {
            _overlay->setPosition(_target_x, 0);
            _overlay = nullptr;
        }
        else
        {
            _overlay->setPosition(pos + sf::Vector2f(adjustment, 0));
        }
    }
}
