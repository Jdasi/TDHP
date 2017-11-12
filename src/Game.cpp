#include "Game.h"
#include "Constants.h"
#include "JTime.h"


Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME)
    , input_handler(nullptr)
    , asset_manager(nullptr)
    , nav_manager(nullptr)
    , game_manager(nullptr)
{
    init();
}


void Game::run()
{
    while (!game_data.exit)
    {
        JTime::tick();

        tick();
        draw();

        input_handler->lateTick();
    }

    window.close();
}


void Game::init()
{
    window.setVerticalSyncEnabled(true);

    initSystems();
    initTextObjects();
}


void Game::initSystems()
{
    input_handler = std::make_unique<InputHandler>(&window);
    game_data.input = input_handler.get();

    asset_manager = std::make_unique<AssetManager>();
    game_data.asset_manager = asset_manager.get();

    nav_manager = std::make_unique<NavManager>();
    game_data.nav_manager = nav_manager.get();

    game_manager = std::make_unique<GameManager>(&game_data);
}


void Game::initTextObjects()
{
    sf::Font* default_font = asset_manager->loadFont(DEFAULT_FONT);

    debug_display.setFont(*default_font);
    debug_display.setCharacterSize(12);
}


void Game::tick()
{
    processEvents(window);

    if (input_handler->getKeyDown(sf::Keyboard::Escape))
    {
        game_data.exit = true;
        return;
    }

    update_timer += JTime::getUnscaledDeltaTime();

    if (update_timer >= 0.1f)
    {
        update_timer = 0;
        updateDebugDisplay();
    }

    nav_manager->tick();
    game_manager->tick();
}


void Game::draw()
{
    window.clear();

    window.draw(debug_display);

    nav_manager->drawBaseLayer(window);
    nav_manager->drawHeatMaps(window);
    game_manager->draw(window);

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
    sf::Event sf_event;
    while (_window.pollEvent(sf_event))
    {
        // Close window: exit
        if (sf_event.type == sf::Event::Closed)
            game_data.exit = true;

        input_handler->processEvent(sf_event);
    }
}
