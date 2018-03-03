#pragma once

#include <memory>

#include "State.h"
#include "Game.h"
#include "GUIButton.h"

struct GameData;

class StateGame final : public State, public GameListener
{
public:
    explicit StateGame(GameData& _game_data);
    virtual ~StateGame() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

private:
    void initObjects();
    void returnToMenu();

    // Game events.
    virtual void onGameOver() override;

    std::unique_ptr<Game> game;
    bool game_over;

    float session_start;
    float session_duration;
    int final_score;

    sf::Text pause_display;
    sf::Sprite game_over_display;

    sf::Text duration_title;
    sf::Text duration_display;

    sf::Text score_title;
    sf::Text score_display;

    GUIButton btn_menu;

};
