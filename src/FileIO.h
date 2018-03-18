#pragma once

#include <vector>
#include <map>
#include <memory>

#include "Level.h"
#include "EnemyType.h"
#include "TowerType.h"

class AssetManager;
struct BrainStatistics;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Functions related to file input / output streams.

---------------------------------------------------------*/
namespace FileIO
{
    std::vector<std::string> enumerateLevelNames();
    std::unique_ptr<LevelData> loadLevelData(const std::string& _name);
    void exportLevel(const Level& _level);
    void exportLevel(const Level& _level, const std::string& _name);

    void exportBrainStatistics(const BrainStatistics& _statistics);

    std::map<std::string, EnemyType> loadEnemyTypes(AssetManager& _asset_manager);
    std::map<std::string, TowerType> loadTowerTypes(AssetManager& _asset_manager);
}
