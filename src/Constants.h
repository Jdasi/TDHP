#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

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

const int WINDOW_LEFT_BOUNDARY          = WINDOW_MARGIN_X;
const int WINDOW_RIGHT_BOUNDARY         = WINDOW_WIDTH - WINDOW_MARGIN_X;
const int WINDOW_TOP_BOUNDARY           = WINDOW_MARGIN_Y;
const int WINDOW_BOTTOM_BOUNDARY        = WINDOW_HEIGHT - WINDOW_MARGIN_Y;

const float PANE_WIDTH                  = WINDOW_RIGHT_BOUNDARY - WINDOW_LEFT_BOUNDARY;
const float PANE_HEIGHT                 = WINDOW_BOTTOM_BOUNDARY - WINDOW_TOP_BOUNDARY;
const sf::Vector2f PANE_SIZE            ({ PANE_WIDTH, PANE_HEIGHT });

// ======================================================================================
// GAMEPLAY SETTINGS
// ======================================================================================
const sf::Color WALKABLE_COLOR          = sf::Color(0, 0, 0, 0);
const sf::Color UNWALKABLE_COLOR        = sf::Color(255, 255, 255, 255);

const int MAX_TOWERS                    = 3;
const int MAX_ENEMIES                   = 100;
const int MAX_ENEMY_SPAWNS              = 10;

// ======================================================================================
// PATHFINDING SETTINGS
// ======================================================================================
const int HEURISTIC_MODIFIER            = 10;

// ======================================================================================
// COMMON PATH STRINGS
// ======================================================================================
const std::string RESOURCES_PATH        { "Resources/" };
const std::string FONTS_PATH            { RESOURCES_PATH + "Fonts/" };
const std::string TEXTURES_PATH         { RESOURCES_PATH + "Images/" };
const std::string AUDIO_PATH            { RESOURCES_PATH + "Audio/" };
const std::string JSON_PATH             { RESOURCES_PATH + "JSON/" };

// ======================================================================================
// JSON STRINGS
// ======================================================================================
const std::string ENEMIES_JSON          { JSON_PATH + "EnemyTypes.json" };

// ======================================================================================
// FONT STRINGS
// ======================================================================================
const std::string DEFAULT_FONT          { "arial.ttf" };

// ======================================================================================
// IMAGE STRINGS
// ======================================================================================
const std::string TOWER_SPRITE          { "tower.png" };
const std::string SPAWN_SPRITE          { "spawn.png" };
const std::string DESTINATION_SPRITE    { "destination.png" };

// ======================================================================================
// AUDIO STRINGS
// ======================================================================================

