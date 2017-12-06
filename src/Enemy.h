#pragma once

#include <SFML/Graphics.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"
#include "Killable.h"

class Enemy final : public Killable, public sf::Sprite,
    public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    void tick();

protected:
    void onSpawn() override;
    void onDeath() override;

private:


};
