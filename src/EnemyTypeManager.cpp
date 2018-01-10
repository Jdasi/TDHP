#include "EnemyTypeManager.h"
#include "FileIO.h"
#include "JMath.h"


EnemyTypeManager::EnemyTypeManager(AssetManager& _asset_manager)
{
    auto type_map = FileIO::loadEnemyTypes(_asset_manager);
    type_entries.reserve(type_map.size());

    for (auto& elem : type_map)
    {
        EnemyTypePair pair;

        pair.key = elem.first;
        pair.type = elem.second;

        type_entries.push_back(pair);
    }
}


EnemyType* EnemyTypeManager::getEnemyType(const std::string& _slug)
{
    for (auto& entry : type_entries)
    {
        if (entry.key != _slug)
            continue;

        return &entry.type;
    }

    return nullptr;
}


// TODO: make this less hacky ..
EnemyType* EnemyTypeManager::getBasicType()
{
    EnemyType* fastest = getFastestType();
    EnemyType* strongest = getStrongestType();

    for (auto& entry : type_entries)
    {
        if (&entry.type == fastest || &entry.type == strongest)
            continue;

        return &entry.type;
    }

    return nullptr;
}


EnemyType* EnemyTypeManager::getFastestType()
{
    EnemyType* fastest_type = nullptr;
    float fastest_speed = 0;

    for (auto& entry : type_entries)
    {
        if (entry.type.speed <= fastest_speed)
            continue;

        fastest_type = &entry.type;
        fastest_speed = entry.type.speed;
    }

    return fastest_type;
}


EnemyType* EnemyTypeManager::getStrongestType()
{
    EnemyType* strongest_type = nullptr;
    int highest_max = 0;

    for (auto& entry : type_entries)
    {
        if (entry.type.max_health <= highest_max)
            continue;

        strongest_type = &entry.type;
        highest_max = entry.type.max_health;
    }

    return strongest_type;
}


EnemyType* EnemyTypeManager::getRandomType()
{
    if (type_entries.empty())
        return nullptr;

    return &type_entries[rand() % type_entries.size()].type;
}
