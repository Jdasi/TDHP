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

    grid->draw(_window);
    heatmap_manager->draw(_window);

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
}


void Game::initSystems()
{
    current_level = FileIO::loadLevel("level3.txt");

    heatmap_manager = std::make_unique<HeatmapManager>(
        current_level.width, current_level.height);

    nav_manager = std::make_unique<NavManager>(*heatmap_manager.get(),
        current_level.width, current_level.height);

    heatmap_manager->createHeatmap(sf::Color::Red, 200, 15);
    heatmap_manager->createHeatmap(sf::Color::Green, 200, 15);

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
    initGrid();
}


void Game::initBorder()
{
    border.setSize(PANE_SIZE);
    border.setFillColor(sf::Color::Blue);
    border.setOutlineThickness(5.0f);
    border.setPosition({ static_cast<float>(WINDOW_LEFT_BOUNDARY),
        static_cast<float>(WINDOW_TOP_BOUNDARY) });
}


void Game::initGridLines()
{
    float rect_width = PANE_WIDTH / current_level.width;
    float rect_height = PANE_HEIGHT / current_level.height;

    float window_left_boundary_f = static_cast<float>(WINDOW_LEFT_BOUNDARY);
    float window_right_boundary_f = static_cast<float>(WINDOW_RIGHT_BOUNDARY);
    float window_top_boundary_f = static_cast<float>(WINDOW_TOP_BOUNDARY);
    float window_bottom_boundary_f = static_cast<float>(WINDOW_BOTTOM_BOUNDARY);

    sf::Color grid_line_color = sf::Color(0, 255, 0, 75);

    // Vertical lines.
    for (int col = 1; col < current_level.width; ++col)
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
    for (int row = 1; row < current_level.height; ++row)
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


void Game::initGrid()
{
    int width = current_level.width;
    int height = current_level.height;

    grid = std::make_unique<TileGrid>(width, height, WALKABLE_COLOR);

    // Parse current_level.
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int index = JHelper::calculateIndex(col, row, width);

            switch (current_level.data[index])
            {
                case 'W': toggleNavNodeWalkable(index); break;
            }
        }
    }
}


void Game::handleContextSelection()
{
    evaluateContextChange(sf::Keyboard::Key::Num1);
    evaluateContextChange(sf::Keyboard::Key::Num2);
    evaluateContextChange(sf::Keyboard::Key::Num3);
    evaluateContextChange(sf::Keyboard::Key::Num4);
    evaluateContextChange(sf::Keyboard::Key::Num5);
    evaluateContextChange(sf::Keyboard::Key::Num6);
    evaluateContextChange(sf::Keyboard::Key::Num7);
    evaluateContextChange(sf::Keyboard::Key::Num8);
    evaluateContextChange(sf::Keyboard::Key::Num9);
    evaluateContextChange(sf::Keyboard::Key::Num0);
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
        {
            return;
        }
        
        int tile_index = JHelper::posToTileIndex(mouse_pos, current_level);
        toggleNavNodeWalkable(tile_index);

        std::cout << "Math tile index: " << tile_index << std::endl;
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

        int tile_index = JHelper::posToTileIndex(mouse_pos, current_level);
        tower_manager->buildTowerAtPos(grid->tileIndexToPos(tile_index));
        std::cout << "Math tile index: " << tile_index << std::endl;
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


void Game::toggleNavNodeWalkable(const int _index)
{
    if (!JHelper::validIndex(_index, current_level.product))
        return;

    nav_manager->toggleTileWalkable(_index);

    bool tile_walkable = nav_manager->isTileWalkable(_index);
    grid->setTileColor(_index, tile_walkable ? WALKABLE_COLOR : UNWALKABLE_COLOR);
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
            return str + "Heat Map " + std::to_string(current_context);
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
