#pragma once

#include <SFML/Graphics.hpp>

#include "EnemyListener.h"
#include "ListenerSubject.h"

class Enemy final : public sf::Sprite, public ListenerSubject<EnemyListener>
{
public:
    Enemy();
    ~Enemy() = default;

    void tick();

    bool isAlive() const;
    void spawn();
    void kill();

private:
    bool alive;

};
