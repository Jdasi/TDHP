#pragma once

#include <SFML/Graphics.hpp>

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
    enum ContextType : int
    {
        NAV,
        GAME,
        HEATMAP_0,
        HEATMAP_1,
        HEATMAP_2,
        HEATMAP_3,
        HEATMAP_4,
        HEATMAP_5,
        HEATMAP_6,
        HEATMAP_7
    };

    void init();

    void handleContextSelection();
    void evaluateContextChange(const sf::Keyboard::Key& _key);
    void processContext();

    std::string contextToString(const ContextType& _context);
    void updateContextDisplay();

    GameData* gd;

    sf::Text context_display;
    ContextType current_context;

    bool painting;

};
