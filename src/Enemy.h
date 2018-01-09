#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"
#include "Killable.h"
#include "LevelPath.h"
#include "TDSprite.h"
#include "EnemyType.h"
#include "HealthBar.h"

class Enemy final : public TDSprite, public Killable,
    public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    EnemyType* getType();
    void setType(EnemyType& _type);

    void tick();
    void draw(sf::RenderWindow& _window) override;

    void setPath(const LevelPath& _path);

protected:
    // Killable events.
    void onSpawn() override;
    void onDamage(TowerType* _attacker_type) override;
    void onDeath(TowerType* _killer_type) override;

    // TDSprite events.
    void onSetPosition() override;

private:
    void initHealthBar();

    LevelPath path;
    int path_index;

    EnemyType* type;
    HealthBar health_bar;

};
