#pragma once

class Killable
{
public:
    Killable()
        : max_health(1)
        , health(0)
    {
    }

    void setMaxHealth(const int _max_health)
    {
        max_health = _max_health;
    }

    bool isAlive() const
    {
        return health > 0;
    }

    void spawn()
    {
        if (isAlive())
            return;

        health = max_health;
        onSpawn();
    }

    // Kill and inform listeners.
    void kill()
    {
        killQuiet();
        onDeath();
    }

    // Kill without informing listeners.
    void killQuiet()
    {
        if (!isAlive())
            return;

        health = 0;
    }

protected:
    virtual void onSpawn() = 0;
    virtual void onDeath() = 0;

private:
    int max_health;
    int health;

};
