#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "InputHandler.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "GameManager.h"

#include "GameData.h"

class Game
{
public:
    Game();
    ~Game() = default;

    void run();

private:
    void init();
    void initSystems();
    void initTextObjects();

    void tick();
    void draw();

    void updateDebugDisplay();
    void processEvents(sf::Window& _window);

    sf::RenderWindow window;

    std::unique_ptr<InputHandler> input_handler;
    std::unique_ptr<AssetManager> asset_manager;
    std::unique_ptr<NavManager> nav_manager;
    std::unique_ptr<GameManager> game_manager;

    GameData game_data;

    // DEBUG.
    sf::Text debug_display;
    float update_timer;

};
