#pragma once

#include <SFML/Graphics.hpp>

class Enemy final : public sf::Sprite
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
