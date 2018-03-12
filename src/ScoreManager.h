#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/Text.hpp>

#include "EnemyListener.h"
#include "TextPopup.h"
#include "Level.h"
#include "Scheduler.h"

namespace sf
{
    class RenderWindow;
}

struct GameData;

class ScoreManager : public EnemyListener
{
public:
    ScoreManager(GameData& _gd, Level& _level);
    ~ScoreManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    float getTimeSurvived() const;
    int getScore() const;

private:
    void initLabels();

    void updateTimeDisplay();

    // Enemy events.
    void onDeath(const Enemy& _caller, TowerType* _killer_type) override;

    int calculateScoreBump(const Enemy& _caller);
    void createTextPopup(const std::string& _str, const sf::Vector2f& _pos);

    GameData& gd;
    Level& level;

    Scheduler scheduler;

    std::vector<std::unique_ptr<TextPopup>> text_popups;

    float time_survived;
    int score;

    sf::Text lbl_time_title;
    sf::Text lbl_time_display;

    sf::Text lbl_score_title;
    sf::Text lbl_score_display;

};
