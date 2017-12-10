#pragma once

class Killable
{
public:
    Killable()
        : alive(false)
    {
    }

    bool isAlive() const
    {
        return alive;
    }

    void spawn()
    {
        if (isAlive())
            return;

        alive = true;
        onSpawn();
    }

    void kill()
    {
        if (!isAlive())
            return;

        alive = false;
        onDeath();
    }

    void killQuiet()
    {
        if (!isAlive())
            return;

        alive = false;
    }

protected:
    virtual void onSpawn() = 0;
    virtual void onDeath() = 0;

private:
    bool alive;

};
