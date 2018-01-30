#pragma once

#include <string>

struct LevelData
{
    std::string name    = "";

    int size_x          = 0;
    int size_y          = 0;
    int product         = 0;

    float tile_width    = 0;
    float tile_height   = 0;

    std::vector<char> raw_data;
};
