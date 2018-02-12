#include "StateGame.h"
#include "GameData.h"
#include "InputHandler.h"
#include "StateHandler.h"
#include "AssetManager.h"


StateGame::StateGame(GameData& _game_data)
    : State(_game_data)
    , game_over(false)
{
    initObjects();
}


void StateGame::onStateEnter()
{
    game = std::make_unique<Game>(gameData());
    game->attachListener(this);

    game_over = false;
}


void StateGame::onStateLeave()
{
    game = nullptr;

    if (JTime::getTimeScale() == 0) // If paused.
        JTime::setTimeScale(1);
}


void StateGame::tick()
{
    if (gameData().input.getKeyDown(sf::Keyboard::P)) // Flip paused status.
        JTime::setTimeScale(JTime::getTimeScale() == 1.f ? 0.f : 1.f);

    if (gameData().input.getKeyDown(sf::Keyboard::F12))
        game->exportCurrentLevel();

    if (gameData().input.getKeyDown(sf::Keyboard::Escape))
        getHandler()->queueState(GameState::SELECTION);

    if (!game_over && JTime::getTimeScale() > 0)
    {
        game->tick();
    }
}


void StateGame::draw(sf::RenderWindow& _window)
{
    game->draw(_window);

    if (!game_over && JTime::getTimeScale() == 0) // If paused.
        _window.draw(pause_display);

    if (game_over)
    {
        _window.draw(game_over_display);

        // TODO: draw score and session info ..
    }
}


void StateGame::initObjects()
{
    sf::Font* default_font = gameData().assets.loadFont(DEFAULT_FONT);

    // Pause display.
    pause_display.setString("PAUSED");
    pause_display.setFont(*default_font);
    pause_display.setCharacterSize(28);
    pause_display.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    pause_display.setFillColor(sf::Color::Red);
    pause_display.setOutlineColor(sf::Color::White);
    pause_display.setOutlineThickness(2);
    JHelper::centerSFOrigin(pause_display);

    // Game Over Display.
    auto* tex = gameData().assets.loadTexture(GAME_OVER_TEXTURE);
    game_over_display.setTexture(*tex);
}


void StateGame::onGameOver()
{
    game_over = true;

    // TODO: Pull score & session info from game ..
}
