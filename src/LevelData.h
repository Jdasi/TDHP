#pragma once

#include <string>

struct LevelData
{
    std::string name        = "";
    std::string description = "";

    int size_x              = 0;
    int size_y              = 0;
    int product             = 0;

    int highest_score       = 0;
    double session_duration = 0;

    float tile_width        = 0;
    float tile_height       = 0;

    std::vector<int> tile_data;
};
