#include <algorithm>

#include "ScoreManager.h"
#include "Enemy.h"
#include "Constants.h"
#include "GameData.h"
#include "AssetManager.h"
#include "GameAudio.h"


ScoreManager::ScoreManager(GameData& _gd, Level& _level)
    : gd(_gd)
    , level(_level)
    , score(0)
{
}


void ScoreManager::tick()
{
    text_popups.erase(std::remove_if(
        text_popups.begin(),
        text_popups.end(),
        [](const auto& _popup) { return _popup->expired(); }),
        text_popups.end());

    for (auto& popup : text_popups)
    {
        popup->tick();
    }
}


void ScoreManager::draw(sf::RenderWindow& _window)
{
    for (auto& popup : text_popups)
    {
        popup->draw(_window);
    }
}


void ScoreManager::onDeath(const Enemy& _caller, TowerType* _killer_type)
{
    int bump = calculateScoreBump(_caller);
    createTextPopup("+" + std::to_string(bump), _caller.getPosition());

    score += bump;
    gd.audio.playSound(SCORE_BUMP_SOUND);
}


int ScoreManager::calculateScoreBump(const Enemy& _caller)
{
    auto ctype = _caller.getType();

    float dist_score = _caller.getDistToGoal() * DIST_SCORE_FACTOR;
    float health_score = ctype->max_health * HEALTH_SCORE_FACTOR;
    float speed_score = ctype->speed * SPEED_SCORE_FACTOR;

    return static_cast<int>(dist_score + health_score + speed_score);
}


void ScoreManager::createTextPopup(const std::string& _str, const sf::Vector2f& _pos)
{
    auto* font = gd.assets.loadFont(DEFAULT_FONT);
    int size = static_cast<int>((level.getTileWidth() * level.getTileHeight()) / 50);

    auto popup = std::make_unique<TextPopup>();
    popup->init(*font, size, _str, sf::Color::White, sf::Color::Black, 2, { 0, -25 }, _pos);

    text_popups.push_back(std::move(popup));
}
