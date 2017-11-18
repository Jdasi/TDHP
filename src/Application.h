#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "InputHandler.h"
#include "AssetManager.h"
#include "Simulation.h"
#include "GameData.h"

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

    void updateDebugDisplay();
    void processEvents(sf::Window& _window);

    sf::RenderWindow window;

    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<AssetManager> asset_manager;
    std::unique_ptr<Simulation> simulation;

    GameData game_data;

    // DEBUG.
    sf::Text debug_display;
    float update_timer;

};
