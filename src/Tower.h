#pragma once

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>

#include "TDSprite.h"
#include "JTime.h"
#include "IProjectileManager.h"
#include "Killable.h"
#include "TowerType.h"
#include "TargetingSystem.h"

class Enemy;
struct Waypoint;

class Tower final : public TDSprite, public Killable
{
public:
    Tower();
    ~Tower() = default;

    void init(IProjectileManager& _iprojectile_manager, Waypoint& _enemy_destination);
    void setType(TowerType& _type);

    void tick(GameData& _gd) override;
    void draw(sf::RenderWindow& _window) override;

    bool canShoot() const;
    float getEngageRadiusSqr() const;

    void updateNearbyEnemies(const std::vector<Enemy*>& _enemies);
    
private:
    // Killable events.
    void onSpawn() override;

    // TDSprite events.
    void onSetPosition() override;

private:
    void initEngageRadius();
    void updateEngageRadius();

    void engage(Enemy* _enemy);
    void shoot(Enemy* _enemy);

    IProjectileManager* iprojectile_manager;
    Waypoint* enemy_destination;
    TowerType* type;

    sf::CircleShape engage_radius_display;
    float last_shot_timestamp;

    float engage_radius;
    float engage_radius_sqr;

    std::vector<Enemy*> nearby_enemies;
    TargetingSystem targeting_system;


};
