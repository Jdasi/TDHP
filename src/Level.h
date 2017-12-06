#pragma once

#include <vector>

struct Level
{
    Level()
        : width(0)
        , height(0)
        , product(0)
    {
    }

    int width;
    int height;
    int product;

    float tile_width;
    float tile_height;

    std::vector<char> data;
};
