#pragma once

#include "AssetManager.h"
#include "NavManager.h"
#include "GameData.h"

#include <SFML/Graphics.hpp>

class Game
{
public:
    Game();
    ~Game() = default;

    void run();

private:
    void init();
    void initTextObjects();

    void tick();
    void draw();

    void updateDebugDisplay();
    void processEvents(sf::Window& _window);

    sf::RenderWindow window;
    AssetManager asset_manager;
    NavManager nav_manager;

    GameData game_data;

    // DEBUG.
    sf::Text debug_display;
    sf::RectangleShape cursor;

    float update_timer;
    bool painting;

    HeatMap* debug_heat_map;

};
