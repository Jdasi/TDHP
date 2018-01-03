#include <string>
#include <iostream>

#include "Game.h"
#include "GameData.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "Constants.h"
#include "JHelper.h"
#include "FileIO.h"


Game::Game(GameData& _gd)
    : gd(_gd)
    , current_level(_gd.level_name)
    , current_context(ContextType::NAV)
    , painting(false)
{
    init();
}


void Game::tick()
{
    // Systems.
    heatmap_manager->tick();
    enemy_director->tick(gd);
    tower_manager->tick();

    // State.
    painting = gd.input.getMouseButton(sf::Mouse::Left);

    handleContextSelection();
    processContext();
}


void Game::draw(sf::RenderWindow& _window)
{
    _window.draw(border);

    heatmap_manager->draw(_window);
    current_level.draw(_window);

    for (auto& arr : grid_lines)
        _window.draw(arr);

    enemy_director->draw(_window);
    tower_manager->draw(_window);

    _window.draw(context_display);
}


void Game::init()
{
    initSystems();
    initObjects();

    ParseCurrentLevel();
}


void Game::initSystems()
{
    TDSprite::init(current_level);

    heatmap_manager = std::make_unique<HeatmapManager>(current_level);

    nav_manager = std::make_unique<NavManager>(*heatmap_manager.get(),
        current_level);

    enemy_director = std::make_unique<EnemyDirector>(gd.asset_manager,
        *nav_manager.get(), *heatmap_manager.get(), current_level);

    tower_manager = std::make_unique<TowerManager>(gd.asset_manager,
        *nav_manager.get(), *enemy_director.get(), current_level);
}


void Game::initObjects()
{
    sf::Font* default_font = gd.asset_manager.loadFont(DEFAULT_FONT);

    // Debug context display.
    context_display.setFont(*default_font);
    context_display.setCharacterSize(16);
    context_display.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 16);

    updateContextDisplay();

    initBorder();
    initGridLines();
}


void Game::initBorder()
{
    auto* texture = gd.asset_manager.loadTexture("grass.jpg");

    border.setTexture(texture);
    border.setSize(PANE_SIZE);
    border.setFillColor(sf::Color(255, 255, 255, 100));
    border.setOutlineThickness(5.0f);
    border.setPosition({ static_cast<float>(WINDOW_LEFT_BOUNDARY),
        static_cast<float>(WINDOW_TOP_BOUNDARY) });
}


void Game::initGridLines()
{
    float rect_width = PANE_WIDTH / current_level.getSizeX();
    float rect_height = PANE_HEIGHT / current_level.getSizeY();

    float window_left_boundary_f = static_cast<float>(WINDOW_LEFT_BOUNDARY);
    float window_right_boundary_f = static_cast<float>(WINDOW_RIGHT_BOUNDARY);
    float window_top_boundary_f = static_cast<float>(WINDOW_TOP_BOUNDARY);
    float window_bottom_boundary_f = static_cast<float>(WINDOW_BOTTOM_BOUNDARY);

    sf::Color grid_line_color = sf::Color(0, 255, 0, 75);

    // Vertical lines.
    for (int col = 1; col < current_level.getSizeX(); ++col)
    {
        auto v_arr = sf::VertexArray(sf::LineStrip, 2);
        auto x_pos = WINDOW_LEFT_BOUNDARY + (rect_width * col);

        v_arr[0].position = sf::Vector2f(x_pos, window_top_boundary_f);
        v_arr[0].color = grid_line_color;

        v_arr[1].position = sf::Vector2f(x_pos, window_bottom_boundary_f);
        v_arr[1].color = grid_line_color;

        grid_lines.push_back(v_arr);
    }

    // Horizontal lines.
    for (int row = 1; row < current_level.getSizeY(); ++row)
    {
        auto v_arr = sf::VertexArray(sf::LineStrip, 2);
        auto y_pos = WINDOW_TOP_BOUNDARY + (rect_height * row);

        v_arr[0].position = sf::Vector2f(window_left_boundary_f, y_pos);
        v_arr[0].color = grid_line_color;

        v_arr[1].position = sf::Vector2f(window_right_boundary_f, y_pos);
        v_arr[1].color = grid_line_color;

        grid_lines.push_back(v_arr);
    }
}


void Game::ParseCurrentLevel()
{
    int size_x = current_level.getSizeX();
    int size_y = current_level.getSizeY();

    for (int row = 0; row < size_y; ++row)
    {
        for (int col = 0; col < size_x; ++col)
        {
            int index = JHelper::calculateIndex(col, row, size_x);

            switch (current_level.getLevelTileType(index))
            {
                case Level::UNWALKABLE:
                {
                    nav_manager->toggleNodeWalkable(index);

                    current_level.setTileColor(index, nav_manager->isNodeWalkable(index) ?
                        WALKABLE_COLOR : UNWALKABLE_COLOR);
                } break;

                case Level::ENEMY_SPAWN:
                {
                    enemy_director->addEnemySpawn(index);
                } break;

                case Level::ENEMY_DESTINATION:
                {
                    enemy_director->setEnemyDestination(index);
                } break;
            }
        }
    }
}


void Game::handleContextSelection()
{
    for (int i = sf::Keyboard::Key::Num0; i <= sf::Keyboard::Key::Num9; ++i)
    {
        evaluateContextChange(static_cast<sf::Keyboard::Key>(i));
    }
}


void Game::evaluateContextChange(const sf::Keyboard::Key& _key)
{
    if (!gd.input.getKeyDown(_key))
        return;

    /* Match selection to the arrangement of number keys on the keyboard.
     * (i.e. shift all numbers one place to the left and make 0 the last selection)
     */
    int selection = static_cast<int>(_key) - static_cast<int>(sf::Keyboard::Key::Num1);

    if (selection == -1)
        selection = 9;

    current_context = static_cast<ContextType>(selection);
    updateContextDisplay();
}


void Game::processContext()
{
    switch (current_context)
    {
        case NAV: { processNavContext(); } break;
        case GAME: { processGameContext(); } break;

        case HEATMAP_0: case HEATMAP_1: case HEATMAP_2: case HEATMAP_3:
        case HEATMAP_4: case HEATMAP_5: case HEATMAP_6: case HEATMAP_7:
        {
            processHeatmapContext();
        } break;

        default: {}
    }
}


void Game::processNavContext()
{
    if (gd.input.getMouseButtonDown(sf::Mouse::Left))
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
            return;
        
        int index = JHelper::posToTileIndex(mouse_pos, current_level);
        nav_manager->toggleNodeWalkable(index);

        current_level.setTileColor(index, nav_manager->isNodeWalkable(index) ?
            WALKABLE_COLOR : UNWALKABLE_COLOR);

        //std::cout << "Math tile index: " << index << std::endl;
    }
    else if (gd.input.getMouseButtonDown(sf::Mouse::Right))
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
            return;

        int index = JHelper::posToTileIndex(mouse_pos, current_level);
        enemy_director->setEnemyDestination(index);
    }
}


void Game::processGameContext()
{
    if (gd.input.getMouseButtonDown(sf::Mouse::Left))
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
        {
            return;
        }

        int index = JHelper::posToTileIndex(mouse_pos, current_level);
        tower_manager->toggleTowerAtPos(JHelper::tileIndexToPos(index, current_level));

        //std::cout << "Math tile index: " << index << std::endl;
    }
}


void Game::processHeatmapContext()
{
    if (painting)
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
        {
            return;
        }

        int tile_index = JHelper::posToTileIndex(mouse_pos, current_level);

        // Only affect the current selected heatmap.
        int heatmap_index = current_context - HEATMAP_0;
        heatmap_manager->paintOnHeatmap(heatmap_index, tile_index, 3);
    }
}


std::string Game::contextToString(const ContextType& _context)
{
    std::string str = "Context: ";

    switch (current_context)
    {
        case NAV:
        {
            return str + "Navigation Mesh";
        } break;

        case GAME:
        {
            return str + "Gameplay";
        }

        case HEATMAP_0: case HEATMAP_1: case HEATMAP_2: case HEATMAP_3:
        case HEATMAP_4: case HEATMAP_5: case HEATMAP_6: case HEATMAP_7:
        {
            int heatmap_id = current_context - HEATMAP_0 + 1;
            return str + "Heat Map " + std::to_string(heatmap_id);
        }

        default:
        {
            return "INVALID_TOKEN";
        }
    }
}


void Game::updateContextDisplay()
{
    context_display.setString(contextToString(current_context));
    JHelper::centerSFOrigin(context_display);
}
