#pragma once

#include <vector>
#include <array>

#include "EnemyType.h"
#include "Enemy.h"
#include "EnemyListener.h"
#include "Constants.h"

class AssetManager;

class EnemyManager : public EnemyListener
{
public:
    EnemyManager(AssetManager& _asset_manager, Waypoint& _enemy_destination);
    ~EnemyManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

    void addEnemyListener(EnemyListener* _listener);
    void spawnEnemy(EnemyType* _type, const sf::Vector2f& _pos, const LevelPath& _path);

    int getNumAlive() const;
    int getNumAliveOfType(EnemyType* _type);

    int getProximityToGoal() const;

    std::vector<Enemy*> getEnemiesNearPosSqr(const sf::Vector2f& _pos,
        const float _radius_sqr);

    bool damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type);
    bool killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type = nullptr);

    void boostEnemyHealth(const int _modifier, const float _duration);
    void boostEnemyHealth(EnemyType* _type, const int _modifier, const float _duration);

    void boostEnemySpeed(const float _modifier, const float _duration);
    void boostEnemySpeed(EnemyType* _type, const float _modifier, const float _duration);

    EnemyType* getEnemyType(const std::string& _slug);
    EnemyType* getFastestType();
    EnemyType* getStrongestType();
    EnemyType* getBasicType();
    EnemyType* getRandomType();

private:
    struct EnemyTypePair
    {
        std::string key;
        EnemyType type;
    };

    void initEnemyTypes(AssetManager& _asset_manager);
    void evaluateFastestType();
    void evaluateStrongestType();
    void evaluateBasicType();

    Enemy* getEnemyAtPos(const sf::Vector2f& _pos);

    // Enemy events.
    void onDeath(const sf::Vector2f& _pos, TowerType* _killer_type) override;
    void onPathComplete(Enemy& _enemy) override;

    std::vector<EnemyTypePair> type_entries;
    std::array<Enemy, MAX_ENEMIES> enemies;
    Waypoint& enemy_destination;

    int num_alive;
    int proximity_to_goal;

    EnemyType* fastest_type;
    EnemyType* strongest_type;
    EnemyType* basic_type;

};
