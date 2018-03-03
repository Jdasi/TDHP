#pragma once

#include <map>

#include "Level.h"
#include "EnemyType.h"
#include "TowerType.h"

class AssetManager;
struct BrainStatistics;

namespace FileIO
{
    LevelData loadLevelData(const std::string& _file_name);
    void exportLevel(const Level& _level);
    void exportLevel(const Level& _level, const std::string& _name);

    void exportBrainStatistics(const BrainStatistics& _statistics);

    std::map<std::string, EnemyType> loadEnemyTypes(AssetManager& _asset_manager);
    std::map<std::string, TowerType> loadTowerTypes(AssetManager& _asset_manager);
}
