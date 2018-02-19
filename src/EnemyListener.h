#pragma once

class Enemy;
struct TowerType;

class EnemyListener
{
public:
    EnemyListener() = default;
    ~EnemyListener() = default;

    virtual void onDeath(const Enemy& _caller, TowerType* _killer_type = nullptr) {}
    virtual void onPathComplete(Enemy& _caller) {}

};
