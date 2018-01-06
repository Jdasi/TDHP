#pragma once

struct TowerType;

class Killable
{
public:
    Killable();

    void setMaxHealth(const int _max_health);

    bool isAlive() const;

    void spawn();

    // Kill and inform listeners.
    void kill(TowerType* _killer_type = nullptr);

    // Kill without informing listeners.
    void killQuiet();

protected:
    virtual void onSpawn() = 0;
    virtual void onDeath(TowerType* _killer_type = nullptr) = 0;

private:
    int max_health;
    int health;

};
