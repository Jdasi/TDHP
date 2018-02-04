#pragma once

#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "InputHandler.h"
#include "AssetManager.h"
#include "GameData.h"
#include "StateHandler.h"
#include "Scheduler.h"

class Application
{
public:
    Application();
    ~Application() = default;

    void main();

private:
    void init();
    void initSystems();
    void initObjects();

    void tick();
    void draw();

    void handleCommonCommands();
    void updateDebugDisplay();
    void processEvents(sf::Window& _window);

    sf::RenderWindow window;

    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<AssetManager> asset_manager;

    std::unique_ptr<StateHandler> state_handler;
    std::unique_ptr<GameData> game_data;

    sf::Text debug_display;
    Scheduler scheduler;

};
