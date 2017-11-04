#pragma once
#include <string>

/*! \file Constants.h
@brief   Constants that are used throughout the game. 
@details Add any new constants to this file. 
*/

// ======================================================================================
// WINDOW SETTINGS
// ======================================================================================
const std::string WINDOW_NAME           { "Tower Defence Heatmap Pathfinding" };

const int WINDOW_WIDTH                  = 800; // Width of the screen.
const int WINDOW_HEIGHT                 = 600; // Height of the screen.
const int WINDOW_MARGIN_X               = 100; // Margin from horizontal screen edges.
const int WINDOW_MARGIN_Y               = 50; // Margin from vertical screen edges.

constexpr float WINDOW_LEFT_BOUNDARY    = WINDOW_MARGIN_X;
constexpr float WINDOW_RIGHT_BOUNDARY   = WINDOW_WIDTH - WINDOW_LEFT_BOUNDARY;
constexpr float WINDOW_TOP_BOUNDARY     = WINDOW_MARGIN_Y;
constexpr float WINDOW_BOTTOM_BOUNDARY  = WINDOW_HEIGHT - WINDOW_MARGIN_Y;

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const int GRID_SIZE_X                   = 50; // Width (in tiles) of the playing grid.
const int GRID_SIZE_Y                   = 50; // Height (in tiles) of the playing grid.
constexpr int GRID_AREA                 = GRID_SIZE_X * GRID_SIZE_Y;

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
// TEXTURE STRINGS
// ======================================================================================


// ======================================================================================
// AUDIO STRINGS
// ======================================================================================

