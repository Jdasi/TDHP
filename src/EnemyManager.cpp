#include "EnemyManager.h"
#include "FileIO.h"
#include "JMath.h"


EnemyManager::EnemyManager(AssetManager& _asset_manager, Waypoint& _enemy_destination)
     : enemy_destination(_enemy_destination)
     , num_alive(0)
     , fastest_type(nullptr)
     , strongest_type(nullptr)
     , basic_type(nullptr)
{
    addEnemyListener(this);
    initEnemyTypes(_asset_manager);
}


void EnemyManager::tick()
{
    proximity_to_goal = JMath::maxInt();

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        auto coords = enemy.getCoords();
        int dist = JHelper::manhattanDistance(coords, enemy_destination.tile_coords);

        if (dist < proximity_to_goal)
        {
            proximity_to_goal = dist;
        }

        enemy.tick();
    }
}


void EnemyManager::draw(sf::RenderWindow& _window)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        enemy.draw(_window);
    }
}


void EnemyManager::addEnemyListener(EnemyListener* _listener)
{
    for (auto& enemy : enemies)
    {
        enemy.attachListener(_listener);
    }
}


void EnemyManager::spawnEnemy(EnemyType* _type, const sf::Vector2f& _pos, const LevelPath& _path)
{
    for (auto& enemy : enemies)
    {
        if (enemy.isAlive())
            continue;

        enemy.setType(*_type);
        enemy.spawn();

        enemy.setPosition(_pos);
        enemy.setPath(_path);

        ++num_alive;

        return;
    }
}


int EnemyManager::getNumAlive() const
{
    return num_alive;
}


int EnemyManager::getNumAliveOfType(EnemyType* _type)
{
    if (_type == nullptr)
        return 0;

    int counter = 0;

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive() || enemy.getType() != _type)
            continue;

        ++counter;
    }

    return counter;
}


int EnemyManager::getProximityToGoal() const
{
    return proximity_to_goal;
}


std::vector<Enemy*> EnemyManager::getEnemiesNearPosSqr(const sf::Vector2f& _pos,
    const float _radius_sqr)
{
    std::vector<Enemy*> nearby_enemies;
    nearby_enemies.reserve(MAX_ENEMIES / 2);

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;

        float dist = JMath::vector2DistanceSqr(enemy.getPosition(), _pos);
        if (dist > _radius_sqr)
            continue;

        nearby_enemies.push_back(&enemy);
    }

    return nearby_enemies;
}


bool EnemyManager::damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type)
{
    Enemy* enemy = getEnemyAtPos(_pos);

    if (enemy == nullptr)
        return false;

    enemy->damage(_attacker_type);

    return true;
}


bool EnemyManager::killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type)
{
    Enemy* enemy = getEnemyAtPos(_pos);

    if (enemy == nullptr)
        return false;

    enemy->kill(_killer_type);

    --num_alive;

    return true;
}


void EnemyManager::boostEnemyHealth(const int _modifier, const float _duration)
{
    boostEnemyHealth(nullptr, _modifier, _duration);
}


void EnemyManager::boostEnemyHealth(EnemyType* _type, const int _modifier, const float _duration)
{
    bool any_type = _type == nullptr;

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive() || (!any_type && enemy.getType() != _type))
            continue;

        enemy.boostHealth(_modifier, _duration);
    }
}


void EnemyManager::boostEnemySpeed(const float _modifier, const float _duration)
{
    boostEnemySpeed(nullptr, _modifier, _duration);
}


void EnemyManager::boostEnemySpeed(EnemyType* _type, const float _modifier, const float _duration)
{
    bool any_type = _type == nullptr;

    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive() || (!any_type && enemy.getType() != _type))
            continue;

        enemy.boostSpeed(_modifier, _duration);
    }
}


EnemyType* EnemyManager::getEnemyType(const std::string& _slug)
{
    for (auto& entry : type_entries)
    {
        if (entry.key != _slug)
            continue;

        return &entry.type;
    }

    return nullptr;
}


EnemyType* EnemyManager::getFastestType()
{
    return fastest_type;
}


EnemyType* EnemyManager::getStrongestType()
{
    return strongest_type;
}


EnemyType* EnemyManager::getBasicType()
{
    return basic_type;
}


EnemyType* EnemyManager::getRandomType()
{
    if (type_entries.empty())
        return nullptr;

    return &type_entries[rand() % type_entries.size()].type;
}


void EnemyManager::initEnemyTypes(AssetManager& _asset_manager)
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

    evaluateFastestType();
    evaluateStrongestType();
    evaluateBasicType();
}


void EnemyManager::evaluateFastestType()
{
    fastest_type = nullptr;
    float fastest_speed = 0;

    for (auto& entry : type_entries)
    {
        if (entry.type.speed <= fastest_speed)
            continue;

        fastest_type = &entry.type;
        fastest_speed = entry.type.speed;
    }
}


void EnemyManager::evaluateStrongestType()
{
    strongest_type = nullptr;
    int highest_max = 0;

    for (auto& entry : type_entries)
    {
        if (entry.type.max_health <= highest_max)
            continue;

        strongest_type = &entry.type;
        highest_max = entry.type.max_health;
    }
}


void EnemyManager::evaluateBasicType()
{
    EnemyType* fastest = getFastestType();
    EnemyType* strongest = getStrongestType();

    for (auto& entry : type_entries)
    {
        if (&entry.type == fastest || &entry.type == strongest)
            continue;

        basic_type = &entry.type;
    }
}


Enemy* EnemyManager::getEnemyAtPos(const sf::Vector2f& _pos)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.isAlive() || !enemy.collisionCheck(_pos))
            continue;

        return &enemy;
    }

    return nullptr;
}


void EnemyManager::onDeath(const sf::Vector2f& _pos, TowerType* _killer_type)
{
    --num_alive;
}


void EnemyManager::onPathComplete(Enemy& _enemy)
{
    --num_alive;
}
