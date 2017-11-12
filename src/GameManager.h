#pragma once

namespace sf
{
    class RenderWindow;
    class Event;
}

struct GameData;
class HeatMap;

class GameManager
{
public:
    GameManager(GameData* _gd);
    ~GameManager() = default;

    void tick();
    void draw(sf::RenderWindow& _window);

private:
    GameData* gd;
    HeatMap* debug_heat_map;

    bool painting;

};
