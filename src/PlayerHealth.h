#pragma once

#include <SFML/Graphics/Text.hpp>

#include "ValueBar.h"
#include "EnemyListener.h"

namespace sf
{
    class RenderWindow;
}

struct GameData;

class PlayerHealth : public EnemyListener
{
public:
    PlayerHealth(GameData& _gd);
    ~PlayerHealth() = default;

    void draw(sf::RenderWindow& _window);

    void damage(const float _amount);
    void heal(const float _amount);

    bool isAlive() const;

private:
    void initHealthBar();
    void initLabels();

    void updateValueBar();

    // Enemy events.
    void onPathComplete(Enemy& _caller) override;

    GameData& gd;

    ValueBar health_bar;
    float health;

    sf::Text lbl_title;

};
