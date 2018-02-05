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
const int MAX_BULLETS                   = 100;

const float MIN_WEIGHTING               = 0;
const float MAX_WEIGHTING               = 255;

const float MAX_BRAIN_ENERGY            = 100;
const float DIRECTOR_ENERGY_REGEN       = 1.5f;
const float ENERGY_ON_DEATH             = 1;
const float ENERGY_ON_PATH_COMPLETION   = 5;

const std::string LASER_TOWER_SLUG      { "LaserTower" };
const std::string BULLET_TOWER_SLUG     { "BulletTower" };

const std::string LASER_PROJ_SLUG       { "laser" };
const std::string BULLET_PROJ_SLUG      { "bullet" };

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
const std::string TOWERS_JSON           { JSON_PATH + "TowerTypes.json" };

// ======================================================================================
// FONT STRINGS
// ======================================================================================
const std::string DEFAULT_FONT          { "arial.ttf" };

// ======================================================================================
// IMAGE STRINGS
// ======================================================================================
const std::string LASER_TOWER_SPRITE    { "laser_tower.png" };
const std::string BULLET_TOWER_SPRITE   { "bullet_tower.png" };
const std::string BULLET_SPRITE         { "bullet.png" };
const std::string SPAWN_SPRITE          { "spawn.png" };
const std::string DESTINATION_SPRITE    { "destination.png" };

const std::string FLOOR_TEXTURE         { "floor.jpg" };
const std::string UNWALKABLE_TEXTURE    { "block.png" };

// ======================================================================================
// AUDIO STRINGS
// ======================================================================================

const std::string BULLET_HIT_SOUND      { "bullet_hit.wav" };
const std::string BULLET_SHOOT_SOUND    { "bullet_shoot.wav" };
const std::string DEST_REACHED_SOUND    { "enemy_destination_reached.wav" };
const std::string HEALTH_BOOST_SOUND    { "enemy_health_boost.wav" };
const std::string SPEED_BOOST_SOUND     { "enemy_speed_boost.wav" };
const std::string GAME_OVER_SOUND       { "game_over.wav" };
const std::string LASER_SHOOT_SOUND     { "laser_shoot.wav" };
const std::string SMOKE_BOMB_SOUND      { "smoke_bomb.wav" };
const std::string TOWER_BOOST_SOUND     { "tower_boost.wav" };
const std::string TOWER_PLACE_SOUND     { "tower_construction.wav" };
const std::string TOWER_DESTROY_SOUND   { "tower_destruction.wav" };
