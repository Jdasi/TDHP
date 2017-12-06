#include "Application.h"
#include "Constants.h"
#include "JTime.h"


Application::Application()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close)
    , input_handler(nullptr)
    , asset_manager(nullptr)
    , game(nullptr)
    , game_data(nullptr)
{
    init();
}


void Application::main()
{
    while (!game_data->exit)
    {
        JTime::tick();

        tick();
        draw();

        input_handler->lateTick();
    }

    window.close();
}


void Application::init()
{
    window.setVerticalSyncEnabled(true);

    initSystems();
    initObjects();
}


void Application::initSystems()
{
    input_handler = std::make_unique<InputHandler>(&window);
    asset_manager = std::make_unique<AssetManager>();

    // Set up game data last as it holds refs to systems.
    game_data = std::make_unique<GameData>(*input_handler.get(),
        *asset_manager.get());

    game_data->level_name = "level3.txt";
}


void Application::initObjects()
{
    sf::Font* default_font = asset_manager->loadFont(DEFAULT_FONT);

    debug_display.setFont(*default_font);
    debug_display.setCharacterSize(12);

    game = std::make_unique<Game>(*game_data.get());
}


void Application::tick()
{
    processEvents(window);

    if (input_handler->getKeyDown(sf::Keyboard::Escape))
    {
        game_data->exit = true;
        return;
    }

    update_timer += JTime::getUnscaledDeltaTime();

    if (update_timer >= 0.1f)
    {
        update_timer = 0;
        updateDebugDisplay();
    }

    game->tick();
}


void Application::draw()
{
    window.clear();

    window.draw(debug_display);
    game->draw(window);

    window.display();
}


void Application::updateDebugDisplay()
{
    std::string debug_str;

    debug_str.append("[Delta Time: " + std::to_string(JTime::getUnscaledDeltaTime()) + "] -- ");
    debug_str.append("[Time: " + std::to_string(JTime::getUnscaledTime()) + "] -- ");
    debug_str.append("[FPS: " + std::to_string(1 / JTime::getUnscaledDeltaTime()) + "]");

    debug_display.setString(debug_str);
}


void Application::processEvents(sf::Window& _window)
{
    sf::Event sf_event;
    while (_window.pollEvent(sf_event))
    {
        // Close window = exit program.
        if (sf_event.type == sf::Event::Closed)
            game_data->exit = true;

        input_handler->processEvent(sf_event);
    }
}
