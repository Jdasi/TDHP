#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"
#include "Killable.h"
#include "LevelPath.h"
#include "TDSprite.h"
#include "EnemyType.h"
#include "ValueBar.h"
#include "Scheduler.h"

class Enemy final : public TDSprite, public Killable,
    public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    EnemyType* getType();
    void setType(EnemyType& _type);

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

    void setPath(const LevelPath& _path);
    void boostHealth(const int _modifier, const float _duration);
    void boostSpeed(const float _modifier, const float _duration);

protected:
    // Killable events.
    void onSpawn() override;
    void onDamage(TowerType* _attacker_type) override;
    void onDeath(TowerType* _killer_type) override;

    // TDSprite events.
    void onSetPosition() override;

private:
    void initHealthBar();

    void resetHealth();
    void resetSpeed();

    LevelPath path;
    int path_index;

    EnemyType* type;
    ValueBar health_bar;

    float speed_modifier;

    Scheduler scheduler;

};
