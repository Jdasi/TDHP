#pragma once

namespace sf
{
    class RenderWindow;
}

struct GameData;

/* Abstract base class for all States used in the game.
 * The StateHandler communicates with all registered objects that derive from this class.
 * 
 * State has a reference to the GameData package so important game information can be
 * updated and passed between states with ease.
 *
 * State also has a reference to its StateHandler to allow for State transitions from
 * within States.
 */
class State
{
friend class StateHandler;

public:
    virtual ~State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;

    virtual void tick() = 0;
    virtual void draw(sf::RenderWindow& _window) = 0;

protected:
    explicit State(GameData& _game_data);

    StateHandler* getHandler() const;
    GameData& gameData() const;

private:
    void setHandler(StateHandler* _handler);

    GameData& game_data;
    StateHandler* handler;
};
