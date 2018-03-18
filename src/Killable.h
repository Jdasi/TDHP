#pragma once

struct TowerType;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Base class for any entity that can be spawned,
damaged, killed, and respawned.

The enclosed virtual functions act as events for derived classes,
which hides base behaviour and allows it to be extended.

---------------------------------------------------------*/
class Killable
{
public:
    Killable();

    int getMaxHealth() const;
    void setMaxHealth(const int _max_health);

    bool isAlive() const;
    float getHealthPercentage() const;

    void spawn();

    // Damage and inform listeners.
    void damage(TowerType* _attacker_type = nullptr);

    // Kill and inform listeners.
    void kill(TowerType* _killer_type = nullptr);

    // Kill without informing listeners.
    void killQuiet();

protected:
    virtual void onSpawn() {};
    virtual void onDamage(TowerType* _attacker_type = nullptr) {};
    virtual void onDeath(TowerType* _killer_type = nullptr) {};

private:
    int max_health;
    int health;

};
