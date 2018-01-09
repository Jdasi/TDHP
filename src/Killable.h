#pragma once

struct TowerType;

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
