#pragma once

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Interface to be extended by classes that wish to observe
Game events.

Subjects must extend: ListenerSubject<GameListener>

---------------------------------------------------------*/
class GameListener
{
public:
    GameListener() = default;
    ~GameListener() = default;

    virtual void onGameOver() {}

};
