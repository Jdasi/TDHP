#include "Application.h"
#include "Constants.h"
#include "JTime.h"
#include "JHelper.h"
#include "GDebugFlags.h"

#include "GameStateSelection.h"
#include "GameStateGame.h"


Application::Application()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, sf::Style::Titlebar | sf::Style::Close)
    , input_handler(nullptr)
    , asset_manager(nullptr)
    , game_data(nullptr)
    , scheduler(Scheduler::TimeUsageType::UNSCALED)
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
    srand(static_cast<unsigned int>(time(0)));

    window.setVerticalSyncEnabled(true);

    initSystems();
    initObjects();

    scheduler.invokeRepeating([this]()
    {
        updateDebugDisplay();
    }, 0, 0.1f);
}


void Application::initSystems()
{
    input_handler = std::make_unique<InputHandler>(&window);
    asset_manager = std::make_unique<AssetManager>();
    game_audio = std::make_unique<GameAudio>(*asset_manager.get());

    // Set up game data last as it holds refs to systems.
    game_data = std::make_unique<GameData>(*input_handler.get(),
        *asset_manager.get(), *game_audio.get());
}


void Application::initObjects()
{
    sf::Font* default_font = asset_manager->loadFont(DEFAULT_FONT);

    backdrop.setTexture(*asset_manager->loadTexture(BACKDROP_TEXTURE));

    debug_display.setFont(*default_font);
    debug_display.setCharacterSize(12);

    state_handler = std::make_unique<GameStateHandler>();

    state_handler->registerState(GAMESTATE_SELECTION, std::make_unique<GameStateSelection>(*game_data.get()));
    state_handler->registerState(GAMESTATE_GAME, std::make_unique<GameStateGame>(*game_data.get()));

    state_handler->queueState(GAMESTATE_SELECTION);
}


void Application::tick()
{
    scheduler.update();

    processEvents(window);
    handleCommonCommands();

    state_handler->tick();
}


void Application::draw()
{
    window.clear();

    window.draw(backdrop);
    state_handler->draw(window);

    if (GDebugFlags::draw_debug_controls)
        window.draw(debug_display);

    window.display();
}


// Debug user commands available throughout the program.
void Application::handleCommonCommands()
{
    if (input_handler->getKeyDown(sf::Keyboard::F1))
    {
        GDebugFlags::draw_debug_controls = !GDebugFlags::draw_debug_controls;
    }

    if (input_handler->getKeyDown(sf::Keyboard::F2))
    {
        GDebugFlags::draw_game = !GDebugFlags::draw_game;
    }

    if (input_handler->getKeyDown(sf::Keyboard::F3))
    {
        GDebugFlags::draw_heatmaps = !GDebugFlags::draw_heatmaps;
    }

    if (input_handler->getKeyDown(sf::Keyboard::F4))
    {
        GDebugFlags::draw_paths = !GDebugFlags::draw_paths;
    }
}


void Application::updateDebugDisplay()
{
    std::string debug_str;

    debug_str.append("[Delta Time: " + std::to_string(JTime::getUnscaledDeltaTime()) + "] -- ");
    debug_str.append("[Time: " + std::to_string(JTime::getUnscaledTime()) + "] -- ");
    debug_str.append("[FPS: " + std::to_string(1 / JTime::getUnscaledDeltaTime()) + "] -- ");
    debug_str.append("[Draw Game: " + JHelper::boolToStr(GDebugFlags::draw_game) + "] -- ");
    debug_str.append("[Draw Heatmaps: " + JHelper::boolToStr(GDebugFlags::draw_heatmaps) + "] -- ");
    debug_str.append("[Draw Paths: " + JHelper::boolToStr(GDebugFlags::draw_paths) + "]");

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
