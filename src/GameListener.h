#pragma once

class GameListener
{
public:
    GameListener() = default;
    ~GameListener() = default;

    virtual void onGameOver() {}

};
