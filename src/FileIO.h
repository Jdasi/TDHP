#pragma once

#include <vector>

#include "Level.h"
#include "EnemyType.h"

class AssetManager;

namespace FileIO
{
    LevelData loadLevelData(const std::string& _file_name);
    std::vector<EnemyType> loadEnemyTypes(AssetManager& _asset_manager);
}
