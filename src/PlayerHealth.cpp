#include "PlayerHealth.h"
#include "Constants.h"
#include "JMath.h"
#include "GameData.h"
#include "GameAudio.h"


PlayerHealth::PlayerHealth(GameData& _gd)
    : gd(_gd)
{
    initHealthBar();
}


void PlayerHealth::draw(sf::RenderWindow& _window)
{
    health_bar.draw(_window);
}


void PlayerHealth::damage(const float _amount)
{
    if (!isAlive())
        return;

    health -= _amount;
    health = JMath::clampf(health, 0, MAX_PLAYER_HEALTH);

    updateValueBar();

    gd.audio.playSound(PLAYER_DAMAGED_SOUND);
}


void PlayerHealth::heal(const float _amount)
{
    health += _amount;
    health = JMath::clampf(health, 0, MAX_PLAYER_HEALTH);

    updateValueBar();
}


bool PlayerHealth::isAlive() const
{
    return health > 0;
}


void PlayerHealth::initHealthBar()
{
    health = MAX_PLAYER_HEALTH;

    health_bar.configure({ PANE_WIDTH, WINDOW_HEIGHT * 0.025f }, 0, sf::Color::Red);
    health_bar.updatePosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT * 0.96f });

    updateValueBar();
}


void PlayerHealth::updateValueBar()
{
    health_bar.updateValuePercentage(health / MAX_PLAYER_HEALTH);
}


void PlayerHealth::onPathComplete(Enemy& _caller)
{
    damage(DAMAGE_ON_GOAL_REACHED);
}
