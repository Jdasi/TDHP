#include "Game.h"
#include "Constants.h"
#include "JTime.h"


Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME)
    , asset_manager()
    , exit(false)
{
    init();
}


void Game::run()
{
    while (!exit)
    {
        JTime::tick();

        tick();
        draw();
    }

    window.close();
}


void Game::init()
{
    window.setVerticalSyncEnabled(true);

    initTextObjects();
}


void Game::initTextObjects()
{
    sf::Font* default_font = asset_manager.loadFont(DEFAULT_FONT);

    delta_time_display.setFont(*default_font);
    delta_time_display.setCharacterSize(16);

    time_display.setFont(*default_font);
    time_display.setCharacterSize(16);
    time_display.setPosition(0, 16);
}


void Game::tick()
{
    processEvents(window);

    // setString every frame is very intensive due to reconstructing textures, try to avoid it.
    delta_time_display.setString("Delta Time: " + std::to_string(JTime::getUnscaledDeltaTime()));
    time_display.setString("Time: " + std::to_string(JTime::getUnscaledTime()));
}


void Game::draw()
{
    window.clear();

    window.draw(delta_time_display);
    window.draw(time_display);

    window.display();
}


void Game::processEvents(sf::Window& _window)
{
    sf::Event event;
    while (_window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            exit = true;

        // Escape key: exit
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
        {
            exit = true;
        }
    }
}
