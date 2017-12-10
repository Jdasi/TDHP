#pragma once

#include <vector>

struct NavPath
{
    NavPath()
        : success(false)
        , total_cost(0)
    {
    }

    std::vector<int> indices;
    bool success;
    int total_cost;
};
