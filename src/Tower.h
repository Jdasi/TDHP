#pragma once

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>

#include "TDSprite.h"
#include "JTime.h"
#include "IProjectileManager.h"
#include "Killable.h"

class Enemy;

class Tower final : public TDSprite, public Killable
{
public:
    Tower();
    ~Tower() = default;

    void init(IProjectileManager& _iprojectile_manager);

    void tick(GameData& _gd) override;
    void draw(sf::RenderWindow& _window) override;

    bool canShoot() const;
    float getEngageRadiusSqr() const;

    void updateNearbyEnemies(const std::vector<Enemy*>& _enemies);
    
private:
    void onSpawn() override;
    void onDeath() override;

    void onSetPosition() override;

private:
    void initEngageRadius();

    Enemy* evaluateCurrentTarget();
    void engage(Enemy* _enemy);
    void shoot(Enemy* _enemy);

    IProjectileManager* iprojectile_manager;

    sf::CircleShape engage_radius_display;
    float last_shot_timestamp;

    float shot_delay;

    float engage_radius;
    float engage_radius_sqr;

    std::vector<Enemy*> nearby_enemies;

};
