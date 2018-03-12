#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

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
const sf::Vector2f WINDOW_SIZE          ({ static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT) });

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
const int MAX_ENEMIES                   = 30;
const int MAX_ENEMY_SPAWNS              = 10;
const int MAX_BULLETS                   = 30;

const float MIN_WEIGHTING               = 0.0f;
const float MAX_WEIGHTING               = 255.0f;

const float MAX_PLAYER_HEALTH           = 100.0f;
const float DAMAGE_ON_GOAL_REACHED      = 15.0f;

const float MAX_BRAIN_ENERGY            = 100.0f;
const float ENERGY_ON_DEATH             = 1.0f;
const float ENERGY_ON_PATH_COMPLETION   = 5.0f;

const float NORMAL_ENERGY_REGEN         = 1.5f;
const float ANGRY_ENERGY_REGEN          = 6.0f;
const float EXHAUSTED_ENERGY_REGEN      = -1.0f;

const float TIME_BEFORE_ANGRY           = 40.0f;
const float BRAIN_ANGRY_TIME            = 30.0f;
const float BRAIN_EXHAUSTED_TIME        = 15.0f;

const float DIST_SCORE_FACTOR           = 10.0f;
const float HEALTH_SCORE_FACTOR         = 0.0f;
const float SPEED_SCORE_FACTOR          = 0.0f;

const std::string LASER_TOWER_SLUG      { "LaserTower" };
const std::string BULLET_TOWER_SLUG     { "BulletTower" };

const std::string LASER_PROJ_SLUG       { "laser" };
const std::string BULLET_PROJ_SLUG      { "bullet" };

const int ONE_STAR_THRESHOLD            = 120;
const int TWO_STAR_THRESHOLD            = 300;
const int THREE_STAR_THRESHOLD          = 600;

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
const std::string LASER_TOWER_TEXTURE   { "laser_tower.png" };
const std::string BULLET_TOWER_TEXTURE  { "bullet_tower.png" };
const std::string BULLET_TEXTURE        { "bullet.png" };
const std::string SPAWN_TEXTURE         { "spawn.png" };
const std::string DESTINATION_TEXTURE   { "destination.png" };

const std::string E_BASIC_TEXTURE       { "enemy_medium.png" };
const std::string E_FAST_TEXTURE        { "enemy_fast.png"};
const std::string E_HEAVY_TEXTURE       { "enemy_heavy.png" };

const std::string FLOOR_TEXTURE         { "floor.jpg" };
const std::string UNWALKABLE_TEXTURE    { "block.png" };

const std::string GAME_OVER_TEXTURE     { "game_over_screen.png" };
const std::string BACKDROP_TEXTURE      { "backdrop.jpg" };

const std::string SLOT_EMPTY_TEXTURE    { "tower_slot_empty.png" };
const std::string SLOT_FILLED_TEXTURE   { "tower_slot_filled.png" };

const std::string BTN_ARROW_TEXTURE     { "btn_arrow.jpg" };
const std::string BTN_PLAY_TEXTURE      { "btn_play.jpg" };
const std::string BTN_QUIT_TEXTURE      { "btn_quit.jpg" };
const std::string BTN_QUIT_S_TEXTURE    { "btn_quit_small.jpg" };

const std::string BS_OVERLAY_TEXTURE    { "brainstate_overlay.png" };
const std::string BS_NORMAL_TEXTURE     { "director_normal.png" };
const std::string BS_ANGRY_TEXTURE      { "director_angry.png" };
const std::string BS_EXHAUSTED_TEXTURE  { "director_exhausted.png" };

// ======================================================================================
// AUDIO STRINGS
// ======================================================================================

const std::string BULLET_HIT_SOUND      { "bullet_hit.wav" };
const std::string BULLET_SHOOT_SOUND    { "bullet_shoot.wav" };
const std::string LASER_SHOOT_SOUND     { "laser_shoot.wav" };

const std::string PLAYER_DAMAGED_SOUND  { "enemy_destination_reached.wav" };
const std::string SCORE_BUMP_SOUND      { "score_bump.wav" };
const std::string GAME_OVER_SOUND       { "game_over.wav" };

const std::string HEALTH_BOOST_SOUND    { "enemy_health_boost.wav" };
const std::string SPEED_BOOST_SOUND     { "enemy_speed_boost.wav" };
const std::string SWARM_SOUND           { "enemy_swarm.wav" };
const std::string SMOKE_BOMB_SOUND      { "smoke_bomb.wav" };

const std::string TOWER_BOOST_SOUND     { "tower_boost.wav" };
const std::string TOWER_PLACE_SOUND     { "tower_construction.wav" };
const std::string TOWER_DESTROY_SOUND   { "tower_destruction.wav" };
const std::string TOWER_ERROR_SOUND     { "too_many_towers.wav" };

const std::string BS_NORMAL_SOUND       { "director_normal.wav" };
const std::string BS_ANGRY_SOUND        { "director_angry.wav" };
const std::string BS_EXHAUSTED_SOUND    { "director_exhausted.wav" };
