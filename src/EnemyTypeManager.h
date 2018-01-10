#pragma once

#include <vector>

#include "EnemyType.h"

class AssetManager;

class EnemyTypeManager
{
public:
    EnemyTypeManager(AssetManager& _asset_manager);
    ~EnemyTypeManager() = default;

    EnemyType* getEnemyType(const std::string& _slug);
    EnemyType* getBasicType();
    EnemyType* getFastestType();
    EnemyType* getStrongestType();
    EnemyType* getRandomType();

private:
    struct EnemyTypePair
    {
        std::string key;
        EnemyType type;
    };

    std::vector<EnemyTypePair> type_entries;

};
