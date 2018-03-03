#include <fstream>
#include <iostream>

#include <jsoncons/json.hpp>

#include "FileIO.h"
#include "Constants.h"
#include "AssetManager.h"
#include "JHelper.h"
#include "BrainStatistics.h"


std::vector<std::string> FileIO::enumerateLevelNames()
{
    std::ifstream file("Resources/Levels/levels.txt");

    std::vector<std::string> names;
    std::string name;

    while (std::getline(file, name))
    {
        names.push_back(name);
    }

    return names;
}


std::unique_ptr<LevelData> FileIO::loadLevelData(const std::string& _name)
{
    using jsoncons::json;

    std::ifstream file("Resources/Levels/" + _name + ".json");
    if (!file.is_open())
    {
        std::cout << "FileIO: Unable to find level file: " << _name.c_str() << std::endl;
        return nullptr;
    }

    json file_data;
    file >> file_data;

    auto ld = std::make_unique<LevelData>();

    ld->name = file_data["name"].as_string();
    ld->description = file_data["description"].as_string();
    ld->size_x = file_data["width"].as_int();
    ld->size_y = file_data["height"].as_int();
    ld->product = ld->size_x * ld->size_y;

    ld->highest_score = file_data["highest_score"].as_int();
    ld->session_duration = file_data["session_duration"].as_double();

    ld->tile_width = PANE_WIDTH / ld->size_x;
    ld->tile_height = PANE_HEIGHT / ld->size_y;

    // Read tile data.
    json tile_data = file_data["tile_data"];
    ld->tile_data.assign(ld->product, 0);

    for (int i = 0; i < ld->product; ++i)
    {
        ld->tile_data[i] = tile_data[i].as_int();
    }

    file.close();

    return std::move(ld);
}


void FileIO::overwriteLevel(LevelData* _ld)
{
}


// Exports the level with a random level name.
void FileIO::exportLevel(const Level& _level)
{
    std::string level_name = std::to_string(rand() % 100000);
    exportLevel(_level, level_name);
}


// Exports (and potentially overwrites) the level with the passed name.
void FileIO::exportLevel(const Level& _level, const std::string& _name)
{
    using jsoncons::json;

    std::string file_name("Resources/Levels/" + _name + ".json");

    int size_x = _level.getSizeX();
    int size_y = _level.getSizeY();

    // Level object.
    json level_info;

    level_info["name"] = _name;
    level_info["description"] = "";
    level_info["width"] = size_x;
    level_info["height"] = size_y;
    level_info["highest_score"] = _level.getHighestScore();
    level_info["session_duration"] = _level.getLongestSurvivedTime();

    // Array of raw tile data.
    json level_array = json::array();
    for (int y = 0; y < size_y; ++y)
    {
        for (int x = 0; x < size_x; ++x)
        {
            int index = JHelper::calculateIndex(x, y, size_x);
            auto tile_type = _level.getTileType(index);

            level_array.push_back(static_cast<int>(tile_type));
        }
    }

    // Put tile data into level_info.
    level_info["tile_data"] = std::move(level_array);

    std::ofstream file(file_name);
    file << level_info;

    file.close();
}


void FileIO::exportBrainStatistics(const BrainStatistics& _statistics)
{
    std::string file_name("brain_statistics.txt");
    std::ofstream file(file_name);

    file
        << "Level Name:            " << _statistics.level_name          << '\n'
        << "Session Duration:      " << _statistics.session_duration    << '\n'
        << '\n'
        << "Decision Points:       " << _statistics.decision_points     << '\n'
        << "No Action:             " << _statistics.no_action_times     << '\n'
        << "Waiting for Energy:    " << _statistics.waiting_times       << '\n'
        << '\n'
        << "Fast Swarm:            " << _statistics.fast_swarm_times    << '\n'
        << "Strong Swarm:          " << _statistics.strong_swarm_times  << '\n'
        << "Basic Swarm:           " << _statistics.basic_swarm_times   << '\n'
        << '\n'
        << "Health Boost (Fast):   " << _statistics.hb_fast_times       << '\n'
        << "Health Boost (Strong): " << _statistics.hb_strong_times     << '\n'
        << "Health Boost (All):    " << _statistics.hb_all_times        << '\n'
        << '\n'
        << "Speed Boost (Fast):    " << _statistics.sb_fast_times       << '\n'
        << "Speed Boost (Strong):  " << _statistics.sb_strong_times     << '\n'
        << "Speed Boost (All):     " << _statistics.sb_all_times        << '\n'
        << '\n'
        << "Smoke Bomb:            " << _statistics.smoke_times         << '\n'
        << '\n'
        << "Completed Paths:       " << _statistics.completed_paths     << '\n';

    file.close();
}


std::map<std::string, EnemyType> FileIO::loadEnemyTypes(AssetManager& _asset_manager)
{
    using jsoncons::json;

    std::map<std::string, EnemyType> types;
    std::ifstream file(ENEMIES_JSON);

    json file_data;
    file >> file_data;

    for (const auto& entry : file_data.object_value())
    {
        auto slug = entry.name();
        const auto& data = entry.value();

        EnemyType enemy_type;

        enemy_type.slug = slug;
        enemy_type.texture = _asset_manager.loadTexture(data["sprite"].as_string());
        enemy_type.speed = static_cast<float>(data["speed"].as_double());
        enemy_type.max_health = data["max_health"].as_int();

        types[slug] = enemy_type;
    }

    return types;
}


std::map<std::string, TowerType> FileIO::loadTowerTypes(AssetManager& _asset_manager)
{
    using jsoncons::json;

    std::map<std::string, TowerType> types;
    std::ifstream file(TOWERS_JSON);

    json file_data;
    file >> file_data;

    for (const auto& entry : file_data.object_value())
    {
        auto slug = entry.name();
        const auto& data = entry.value();

        TowerType tower_type;

        // General data.
        tower_type.slug = slug;
        tower_type.texture = _asset_manager.loadTexture(data["sprite"].as_string());
        tower_type.engage_radius = static_cast<float>(data["engage_radius"].as_double());
        tower_type.shot_delay = static_cast<float>(data["shot_delay"].as_double());

        // Process projectile stats.
        json proj_data = data["projectile_stats"];
        auto& projectile_stats = tower_type.projectile_stats;

        projectile_stats.slug = proj_data["slug"].as_string();
        projectile_stats.damage = proj_data["damage"].as_int();

        if (proj_data.has_key("speed"))
        {
            projectile_stats.speed = static_cast<float>(proj_data["speed"].as_double());
        }

        // Process targeting prefs.
        json prefs_data = data["targeting_prefs"];
        auto& targeting_prefs = tower_type.targeting_prefs;

        targeting_prefs.max_health_factor = static_cast<float>(prefs_data["max_health_factor"].as_double());
        targeting_prefs.speed_factor = static_cast<float>(prefs_data["speed_factor"].as_double());
        targeting_prefs.self_distance_factor = static_cast<float>(prefs_data["self_distance_factor"].as_double());
        targeting_prefs.base_distance_factor = static_cast<float>(prefs_data["base_distance_factor"].as_double());

        // Add the type.
        types[slug] = tower_type;
    }

    return types;
}
