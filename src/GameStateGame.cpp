#include <iostream>

#include "GameStateGame.h"
#include "GameStateHandler.h"
#include "GameData.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "GameAudio.h"


GameStateGame::GameStateGame(GameData& _game_data)
    : GameState(_game_data)
    , game_over(false)
    , session_start(0)
    , session_duration(0)
    , final_score(0)
{
    initObjects();
}


void GameStateGame::onStateEnter()
{
    game = std::make_unique<Game>(gameData());
    game->attachListener(this);

    game_over = false;
    session_start = JTime::getTime();
    session_duration = 0;
    final_score = 0;
}


void GameStateGame::onStateLeave()
{
    game = nullptr;

    // Make sure game isn't paused when we leave.
    if (JTime::getTimeScale() == 0)
        JTime::setTimeScale(1);
}


void GameStateGame::tick()
{
    if (gameData().input.getKeyDown(sf::Keyboard::P)) // Flip paused status.
        JTime::setTimeScale(JTime::getTimeScale() == 1.f ? 0.f : 1.f);

    if (gameData().input.getKeyDown(sf::Keyboard::F12))
        game->exportLevelAsNew();

    if (gameData().input.getKeyDown(sf::Keyboard::Escape))
    {
        returnToMenu();
    }

    if (!game_over && JTime::getTimeScale() > 0)
    {
        game->tick();
    }

    btn_menu.tick(gameData());
}


void GameStateGame::draw(sf::RenderWindow& _window)
{
    game->draw(_window);

    if (!game_over && JTime::getTimeScale() == 0) // If paused.
        _window.draw(pause_display);

    if (game_over)
    {
        _window.draw(game_over_display);

        _window.draw(duration_title);
        _window.draw(duration_display);

        _window.draw(score_title);
        _window.draw(score_display);
    }

    btn_menu.draw(_window);
}


void GameStateGame::initObjects()
{
    auto* font = gameData().assets.loadFont(DEFAULT_FONT);

    // Pause display.
    pause_display.setFont(*font);
    pause_display.setCharacterSize(28);
    pause_display.setString("PAUSED");
    pause_display.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pause_display.setFillColor(sf::Color::Red);
    pause_display.setOutlineColor(sf::Color::White);
    pause_display.setOutlineThickness(2);

    JHelper::centerSFOrigin(pause_display);

    // Game Over Display.
    auto* tex = gameData().assets.loadTexture(GAME_OVER_TEXTURE);
    game_over_display.setTexture(*tex);

    // Duration Display.
    duration_title.setFont(*font);
    duration_title.setCharacterSize(28);
    duration_title.setString("Session Duration:");
    duration_title.setPosition(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.35f);
    duration_title.setOutlineColor(sf::Color::Black);
    duration_title.setOutlineThickness(2);

    duration_display.setFont(*font);
    duration_display.setCharacterSize(28);
    duration_display.setString("0");
    duration_display.setPosition(WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.35f);
    duration_display.setOutlineColor(sf::Color::Black);
    duration_display.setOutlineThickness(2);

    // Score Display.
    score_title.setFont(*font);
    score_title.setCharacterSize(28);
    score_title.setString("Final Score:");
    score_title.setPosition(WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.4f);
    score_title.setOutlineColor(sf::Color::Black);
    score_title.setOutlineThickness(2);

    score_display.setFont(*font);
    score_display.setCharacterSize(28);
    score_display.setString("0");
    score_display.setPosition(WINDOW_WIDTH * 0.6f, WINDOW_HEIGHT * 0.4f);
    score_display.setOutlineColor(sf::Color::Black);
    score_display.setOutlineThickness(2);

    // Button to return to level selection.
    btn_menu.setPosition(WINDOW_WIDTH - 25, 25);
    btn_menu.setColors(sf::Color(255, 0, 0, 255), sf::Color(255, 200, 200, 255), sf::Color(100, 0, 0, 255));
    btn_menu.setTexture(gameData().assets.loadTexture(BTN_QUIT_S_TEXTURE));
    btn_menu.setSize(sf::Vector2f(50, 50));
    btn_menu.addClickEvent([this]() { returnToMenu(); });
}


void GameStateGame::returnToMenu()
{
    gameData().audio.playSound(TOWER_ERROR_SOUND);
    getHandler()->queueState(GAMESTATE_SELECTION);
}


void GameStateGame::onGameOver()
{
    game_over = true;

    session_duration = JTime::getTime() - session_start;
    final_score = game->getScore();

    duration_display.setString(JHelper::timeToStr(session_duration));
    score_display.setString(std::to_string(final_score));

    auto* ld = gameData().selected_level_data;
    if (final_score > ld->highest_score)
    {
        ld->highest_score = final_score;
        ld->session_duration = session_duration;

        game->exportLevel();
    }
}
