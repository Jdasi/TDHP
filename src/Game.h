#pragma once

#include "AssetManager.h"

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

    void processEvents(sf::Window& _window);

    sf::RenderWindow window;
    AssetManager asset_manager;

    sf::Text delta_time_display;
    sf::Text time_display;

    bool exit;

};
