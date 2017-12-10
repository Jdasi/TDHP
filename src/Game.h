#pragma once

#include <memory>

#include <SFML/Window.hpp>

#include "HeatmapManager.h"
#include "NavManager.h"
#include "EnemyDirector.h"
#include "TowerManager.h"
#include "Level.h"

struct GameData;

class Game
{
public:
    Game(GameData& _gd);
    ~Game() = default;

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
    void initSystems();
    void initObjects();

    void initBorder();
    void initGridLines();
    void ParseCurrentLevel();

    void handleContextSelection();
    void evaluateContextChange(const sf::Keyboard::Key& _key);
    void processContext();

    void processNavContext();
    void processGameContext();
    void processHeatmapContext();

    std::string contextToString(const ContextType& _context);
    void updateContextDisplay();

    std::unique_ptr<HeatmapManager> heatmap_manager;
    std::unique_ptr<NavManager> nav_manager;
    std::unique_ptr<EnemyDirector> enemy_director;
    std::unique_ptr<TowerManager> tower_manager;

    Level current_level;

    GameData& gd;
    ContextType current_context;

    sf::RectangleShape border;
    std::vector<sf::VertexArray> grid_lines;
    sf::Text context_display;

    bool painting;

};
