#include <fstream>
#include <iostream>

#include <jsoncons/json.hpp>

#include "FileIO.h"
#include "Constants.h"
#include "AssetManager.h"


LevelData FileIO::loadLevelData(const std::string& _file_name)
{
    LevelData ld;

    std::ifstream file("Resources/Levels/" + _file_name);
    if (!file.is_open())
    {
        std::cout << "Unable to find level file: " << _file_name.c_str() << std::endl;
        return ld;
    }

    file >> ld.size_x >> ld.size_y;
    ld.product = ld.size_x * ld.size_y;

    ld.tile_width = PANE_WIDTH / ld.size_x;
    ld.tile_height = PANE_HEIGHT / ld.size_y;

    ld.raw_data.reserve(ld.product);
    char elem = 0;

    for (int row = 0; row < ld.size_y; ++row)
    {
        for (int col = 0; col < ld.size_x; ++col)
        {
            file >> elem;
            ld.raw_data.push_back(elem);
        }
    }

    return ld;
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
        enemy_type.move_speed = static_cast<float>(data["move_speed"].as_double());
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

        tower_type.slug = slug;
        tower_type.texture = _asset_manager.loadTexture(data["sprite"].as_string());
        tower_type.projectile_slug = data["projectile_slug"].as_string();

        if (data.has_key("projectile_speed"))
        {
            tower_type.projectile_stats.move_speed = static_cast<float>(data["projectile_speed"].as_double());
        }

        tower_type.projectile_stats.damage = data["projectile_damage"].as_int();
        tower_type.engage_radius = static_cast<float>(data["engage_radius"].as_double());
        tower_type.shot_delay = static_cast<float>(data["shot_delay"].as_double());

        types[slug] = tower_type;
    }

    return types;
}
