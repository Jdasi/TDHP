#pragma once

#include <vector>
#include <memory>

#include <SFML/Graphics/Text.hpp>

#include "EnemyListener.h"
#include "TextPopup.h"
#include "Level.h"

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

    int getScore() const;

private:
    void initLabels();

    // Enemy events.
    void onDeath(const Enemy& _caller, TowerType* _killer_type) override;

    int calculateScoreBump(const Enemy& _caller);
    void createTextPopup(const std::string& _str, const sf::Vector2f& _pos);

    GameData& gd;
    Level& level;

    std::vector<std::unique_ptr<TextPopup>> text_popups;
    int score;

    sf::Text lbl_title;
    sf::Text lbl_score;

};
