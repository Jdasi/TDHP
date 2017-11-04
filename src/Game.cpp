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

    cursor.setSize(sf::Vector2f(10, 10));
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setOrigin(sf::Vector2f(5, 5));
}


void Game::initTextObjects()
{
    sf::Font* default_font = asset_manager.loadFont(DEFAULT_FONT);

    debug_display.setFont(*default_font);
    debug_display.setCharacterSize(12);
}


void Game::tick()
{
    processEvents(window);

    update_timer += JTime::getUnscaledDeltaTime();

    if (update_timer >= 0.1f)
    {
        update_timer = 0;
        updateDebugDisplay();
    }

    cursor.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}


void Game::draw()
{
    window.clear();

    window.draw(debug_display);
    nav_manager.draw(window);
    window.draw(cursor);

    window.display();
}


void Game::updateDebugDisplay()
{
    std::string debug_str;

    debug_str.append("[Delta Time: " + std::to_string(JTime::getUnscaledDeltaTime()) + "] -- ");
    debug_str.append("[Time: " + std::to_string(JTime::getUnscaledTime()) + "] -- ");
    debug_str.append("[FPS: " + std::to_string(1 / JTime::getUnscaledDeltaTime()) + "]");

    debug_display.setString(debug_str);
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

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                nav_manager.colorTileAtPos(cursor.getPosition());
            }
        }
    }
}
