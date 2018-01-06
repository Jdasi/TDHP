#pragma once

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>

#include "TDSprite.h"
#include "JTime.h"
#include "IProjectileManager.h"
#include "Killable.h"
#include "TowerType.h"

class Enemy;

class Tower final : public TDSprite, public Killable
{
public:
    Tower();
    ~Tower() = default;

    void init(IProjectileManager& _iprojectile_manager);
    void setType(TowerType& _type);

    void tick(GameData& _gd) override;
    void draw(sf::RenderWindow& _window) override;

    bool canShoot() const;
    float getEngageRadiusSqr() const;

    void updateNearbyEnemies(const std::vector<Enemy*>& _enemies);
    
private:
    void onSpawn() override;
    void onDeath(TowerType* _killer_type) override;

    void onSetPosition() override;

private:
    void initEngageRadius();
    void updateEngageRadius();

    Enemy* evaluateCurrentTarget();
    void engage(Enemy* _enemy);
    void shoot(Enemy* _enemy);

    IProjectileManager* iprojectile_manager;
    TowerType* type;

    sf::CircleShape engage_radius_display;
    float last_shot_timestamp;

    float engage_radius;
    float engage_radius_sqr;

    std::vector<Enemy*> nearby_enemies;

};
