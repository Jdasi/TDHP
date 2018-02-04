#pragma once

#include <memory>

#include "State.h"
#include "Game.h"

struct GameData;

class StateGame final : public State
{
public:
    explicit StateGame(GameData& _game_data);
    virtual ~StateGame() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

private:
    std::unique_ptr<Game> game;

    sf::Text pause_display;

};
