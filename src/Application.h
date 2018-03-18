#pragma once

#include <memory>

#include <SFML/Window.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "InputHandler.h"
#include "AssetManager.h"
#include "GameAudio.h"
#include "GameData.h"
#include "GameStateHandler.h"
#include "GameState.h"
#include "Scheduler.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Main point of entry into the program.

Uses a state system to control the flow of the program
between different game states.

Core game systems are loaded into a GameData struct for use in
different areas of the program.

---------------------------------------------------------*/
class Application final
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
    std::unique_ptr<GameAudio> game_audio;

    std::unique_ptr<GameStateHandler> state_handler;
    std::unique_ptr<GameData> game_data;

    sf::Sprite backdrop;
    sf::Text debug_display;
    Scheduler scheduler;

};
