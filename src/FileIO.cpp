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


std::vector<EnemyType> FileIO::loadEnemyTypes(AssetManager& _asset_manager)
{
    using jsoncons::json;

    std::vector<EnemyType> types;
    std::ifstream file(ENEMIES_JSON);

    json file_data;
    file >> file_data;

    for (const auto& entry : file_data.object_value())
    {
        auto slug = entry.name();
        const auto& data = entry.value();

        sf::Texture* sprite = _asset_manager.loadTexture(data["sprite"].as_string());
        float speed = static_cast<float>(data["move_speed"].as_double());
        int health = data["max_health"].as_int();

        types.emplace_back(slug, sprite, speed, health);
    }

    return types;
}
