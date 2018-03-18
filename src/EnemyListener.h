#pragma once

class Enemy;
struct TowerType;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Interface to be extended by classes that wish to observe
Enemy events.

Subjects must extend: ListenerSubject<EnemyListener>

---------------------------------------------------------*/
class EnemyListener
{
public:
    EnemyListener() = default;
    ~EnemyListener() = default;

    virtual void onDeath(const Enemy& _caller, TowerType* _killer_type = nullptr) {}
    virtual void onPathComplete(Enemy& _caller) {}

};
