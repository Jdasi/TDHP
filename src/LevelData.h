#pragma once

struct LevelData
{
    LevelData()
        : size_x(0)
        , size_y(0)
        , product(0)
    {
    }

    int size_x;
    int size_y;
    int product;

    float tile_width;
    float tile_height;

    std::vector<char> raw_data;
};
