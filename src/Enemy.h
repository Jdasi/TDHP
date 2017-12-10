#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"
#include "Killable.h"
#include "LevelPath.h"

class Enemy final : public Killable, public sf::Sprite,
    public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    void tick();

    void setPath(const LevelPath& _path);

protected:
    void onSpawn() override;
    void onDeath() override;

private:
    LevelPath path;
    int path_index;

};
