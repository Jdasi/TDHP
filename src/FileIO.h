#pragma once

#include <vector>

#include "Level.h"

namespace FileIO
{
    LevelData loadLevelData(const std::string& _file_name);
}
