#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"
#include "Killable.h"
#include "LevelPath.h"
#include "TDSprite.h"
#include "EnemyType.h"

class Enemy final : public TDSprite, public Killable,
    public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    void setType(EnemyType& _type);

    void tick();
    void draw(sf::RenderWindow& _window) override;

    void setPath(const LevelPath& _path);

protected:
    void onSpawn() override;
    void onDeath(TowerType* _killer_type) override;

private:
    LevelPath path;
    int path_index;

    EnemyType* type;

};
