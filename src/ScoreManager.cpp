#include <algorithm>

#include "ScoreManager.h"
#include "Enemy.h"
#include "Constants.h"
#include "GameData.h"
#include "AssetManager.h"
#include "GameAudio.h"
#include "JTime.h"
#include "JHelper.h"
#include "JMath.h"


ScoreManager::ScoreManager(GameData& _gd, Level& _level)
    : gd(_gd)
    , level(_level)
    , time_survived(0)
    , score(0)
{
    scheduler.invokeRepeating([this]()
    {
        updateTimeDisplay();
    }, 0, 1);

    initLabels();
}


void ScoreManager::tick()
{
    scheduler.update();
    cleanUpExpiredText();

    time_survived += JTime::getDeltaTime();

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

    _window.draw(lbl_time_title);
    _window.draw(lbl_time_display);

    _window.draw(lbl_score_title);
    _window.draw(lbl_score_display);
}


float ScoreManager::getTimeSurvived() const
{
    return time_survived;
}


int ScoreManager::getScore() const
{
    return score;
}


void ScoreManager::initLabels()
{
    // Time survived display.
    lbl_time_title.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_time_title.setCharacterSize(14);
    lbl_time_title.setStyle(sf::Text::Bold);
    lbl_time_title.setFillColor(sf::Color::White);
    lbl_time_title.setPosition({ WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT * 0.945f });
    lbl_time_title.setString("Survived:");

    lbl_time_display.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_time_display.setCharacterSize(14);
    lbl_time_display.setStyle(sf::Text::Bold);
    lbl_time_display.setFillColor(sf::Color::White);
    lbl_time_display.setPosition({ lbl_time_title.getPosition().x + 75, WINDOW_HEIGHT * 0.945f });
    lbl_time_display.setString("00:00:00");

    // Score display.
    lbl_score_title.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_score_title.setCharacterSize(14);
    lbl_score_title.setStyle(sf::Text::Bold);
    lbl_score_title.setFillColor(sf::Color::White);
    lbl_score_title.setPosition({ WINDOW_WIDTH * 0.75f, WINDOW_HEIGHT * 0.967f });
    lbl_score_title.setString("Score:");

    lbl_score_display.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_score_display.setCharacterSize(14);
    lbl_score_display.setStyle(sf::Text::Bold);
    lbl_score_display.setFillColor(sf::Color::White);
    lbl_score_display.setPosition({ lbl_score_title.getPosition().x + 75, WINDOW_HEIGHT * 0.967f });
    lbl_score_display.setString("0");
}


void ScoreManager::updateTimeDisplay()
{
    lbl_time_display.setString(JHelper::timeToStr(time_survived));
}


void ScoreManager::cleanUpExpiredText()
{
    text_popups.erase(std::remove_if(
        text_popups.begin(),
        text_popups.end(),
        [](const auto& _popup) { return _popup->expired(); }),
        text_popups.end());
}


void ScoreManager::onDeath(const Enemy& _caller, TowerType* _killer_type)
{
    int bump = calculateScoreBump(_caller);
    createTextPopup("+" + std::to_string(bump), _caller.getPosition());

    score += bump;
    lbl_score_display.setString(std::to_string(score));

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
