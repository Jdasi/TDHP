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
    initLabels();
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

    _window.draw(lbl_title);
    _window.draw(lbl_score);
}


void ScoreManager::initLabels()
{
    lbl_title.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_title.setCharacterSize(14);
    lbl_title.setStyle(sf::Text::Bold);
    lbl_title.setFillColor(sf::Color::White);
    lbl_title.setPosition({ WINDOW_WIDTH * 0.8f, WINDOW_HEIGHT * 0.96f });
    lbl_title.setString("Score:");

    JHelper::centerSFOrigin(lbl_title);

    lbl_score.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_score.setCharacterSize(14);
    lbl_score.setStyle(sf::Text::Bold);
    lbl_score.setFillColor(sf::Color::White);
    lbl_score.setPosition({ lbl_title.getPosition().x + 30, WINDOW_HEIGHT * 0.949f });
    lbl_score.setString("0");
}


void ScoreManager::onDeath(const Enemy& _caller, TowerType* _killer_type)
{
    int bump = calculateScoreBump(_caller);
    createTextPopup("+" + std::to_string(bump), _caller.getPosition());

    score += bump;
    lbl_score.setString(std::to_string(score));

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
