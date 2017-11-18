#pragma once

#include <string>

#include <SFML/Graphics.hpp>

/*! \file Constants.h
@brief   Constants that are used throughout the game. 
@details Add any new constants to this file. 
*/

// ======================================================================================
// WINDOW SETTINGS
// ======================================================================================
const std::string WINDOW_NAME           { "Tower Defence Heatmap Pathfinding" };

const int WINDOW_WIDTH                  = 1024; // Width of the screen.
const int WINDOW_HEIGHT                 = 768; // Height of the screen.
const int WINDOW_MARGIN_X               = 100; // Margin from horizontal screen edges.
const int WINDOW_MARGIN_Y               = 50; // Margin from vertical screen edges.

constexpr int WINDOW_LEFT_BOUNDARY    = WINDOW_MARGIN_X;
constexpr int WINDOW_RIGHT_BOUNDARY   = WINDOW_WIDTH - WINDOW_LEFT_BOUNDARY;
constexpr int WINDOW_TOP_BOUNDARY     = WINDOW_MARGIN_Y;
constexpr int WINDOW_BOTTOM_BOUNDARY  = WINDOW_HEIGHT - WINDOW_MARGIN_Y;

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const float TILE_PADDING                = 2;

constexpr float PANE_WIDTH              = WINDOW_RIGHT_BOUNDARY - WINDOW_LEFT_BOUNDARY;
constexpr float PANE_HEIGHT             = WINDOW_BOTTOM_BOUNDARY - WINDOW_TOP_BOUNDARY;
const sf::Vector2f PANE_SIZE            ({ PANE_WIDTH, PANE_HEIGHT });

const sf::Color WALKABLE_COLOR          = sf::Color(0, 0, 0, 255);
const sf::Color UNWALKABLE_COLOR        = sf::Color(255, 255, 255, 255);

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================
const std::string RESOURCES_PATH        { "Resources/" };
const std::string FONTS_PATH            { RESOURCES_PATH + "Fonts/" };
const std::string TEXTURES_PATH         { RESOURCES_PATH + "Images/" };
const std::string AUDIO_PATH            { RESOURCES_PATH + "Audio/" };

// ======================================================================================
// FONT STRINGS
// ======================================================================================
const std::string DEFAULT_FONT          { "arial.ttf" };

// ======================================================================================
// IMAGE STRINGS
// ======================================================================================


// ======================================================================================
// AUDIO STRINGS
// ======================================================================================

