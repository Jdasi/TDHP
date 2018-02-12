#pragma once

#include <memory>

#include "State.h"
#include "Game.h"

struct GameData;

class StateGame final : public State, public GameListener
{
public:
    explicit StateGame(GameData& _game_data);
    virtual ~StateGame() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

private:
    void initObjects();

    // Game events.
    virtual void onGameOver() override;

    std::unique_ptr<Game> game;
    bool game_over;

    sf::Text pause_display;
    sf::Sprite game_over_display;

};
