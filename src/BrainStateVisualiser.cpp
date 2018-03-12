#include <SFML/Graphics/RenderWindow.hpp>

#include "BrainStateVisualiser.h"
#include "BrainStateType.h"
#include "GameData.h"
#include "AssetManager.h"
#include "JTime.h"
#include "Constants.h"


BrainStateVisualiser::BrainStateVisualiser(GameData& _gd)
    : gd(_gd)
    , overlay_x_offset(200)
    , half_overlay_x_offset(overlay_x_offset / 2)
    , overlay_slide_speed(1000)
{
    init();
}


void BrainStateVisualiser::tick()
{
    handleSlide();
}


void BrainStateVisualiser::draw(sf::RenderWindow& _window)
{
    _window.draw(angry_overlay);
    _window.draw(exhausted_overlay);
}


void BrainStateVisualiser::stateChanged(int _state_key)
{
    switch (_state_key)
    {
        case BRAINSTATE_NORMAL:
        {
            angry_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);
            exhausted_overlay.setPosition(-overlay_x_offset, 0);

            sliding_in_overlay = nullptr;
            sliding_out_overlay = &exhausted_overlay;
        } break;

        case BRAINSTATE_ANGRY:
        {
            angry_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);
            exhausted_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);

            sliding_in_overlay = &angry_overlay;
            sliding_out_overlay = nullptr;
        } break;

        case BRAINSTATE_EXHAUSTED:
        {
            angry_overlay.setPosition(-half_overlay_x_offset, 0);
            exhausted_overlay.setPosition(-WINDOW_WIDTH - overlay_x_offset, 0);

            sliding_in_overlay = &exhausted_overlay;
            sliding_out_overlay = &angry_overlay;
        } break;
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
