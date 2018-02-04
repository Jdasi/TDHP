#pragma once

#include "State.h"

struct GameData;

class StateSelection final : public State
{
public:
    explicit StateSelection(GameData& _game_data);
    virtual ~StateSelection() = default;

    void onStateEnter() override;
    void onStateLeave() override;

    void tick() override;
    void draw(sf::RenderWindow& _window) override;

private:

};
