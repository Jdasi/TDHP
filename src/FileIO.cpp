#include <fstream>
#include <iostream>

#include "FileIO.h"
#include "Constants.h"


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
