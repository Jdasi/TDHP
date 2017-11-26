#include <string>
#include <iostream>

#include "Simulation.h"
#include "GameData.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "Constants.h"
#include "JHelper.h"
#include "FileIO.h"


Simulation::Simulation(GameData* _gd)
    : gd(_gd)
    , current_context(ContextType::NAV)
    , painting(false)
{
    init();
}


void Simulation::tick()
{
    // Systems.
    heatmap_manager->tick();
    enemy_director->tick(gd);
    tower_manager->tick();

    // State.
    painting = gd->input->getMouseButton(sf::Mouse::Left);

    handleContextSelection();
    processContext();
}


void Simulation::draw(sf::RenderWindow& _window)
{
    _window.draw(border);

    nav_manager->draw(_window);
    heatmap_manager->draw(_window);

    for (auto& arr : grid_lines)
        _window.draw(arr);

    enemy_director->draw(_window);
    tower_manager->draw(_window);

    _window.draw(context_display);
}


void Simulation::init()
{
    initSystems();
    initObjects();
}


void Simulation::initSystems()
{
    current_level = FileIO::loadLevel("level3.txt");

    heatmap_manager = std::make_unique<HeatmapManager>(
        current_level.width, current_level.height);

    nav_manager = std::make_unique<NavManager>(heatmap_manager.get());
    nav_manager->parseLevel(current_level);

    heatmap_manager->createHeatmap(sf::Color::Red, 200, 15);
    heatmap_manager->createHeatmap(sf::Color::Green, 200, 15);

    enemy_director = std::make_unique<EnemyDirector>(gd->asset_manager,
        nav_manager.get(), heatmap_manager.get(), &current_level);

    tower_manager = std::make_unique<TowerManager>(gd->asset_manager,
        nav_manager.get(), enemy_director.get(), &current_level);


}


void Simulation::initObjects()
{
    sf::Font* default_font = gd->asset_manager->loadFont(DEFAULT_FONT);

    // Debug context display.
    context_display.setFont(*default_font);
    context_display.setCharacterSize(16);
    context_display.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 16);

    updateContextDisplay();

    // Border.
    border.setSize(PANE_SIZE);
    border.setFillColor(sf::Color::Blue);
    border.setOutlineThickness(5.0f);

    border.setPosition({ static_cast<float>(WINDOW_LEFT_BOUNDARY),
        static_cast<float>(WINDOW_TOP_BOUNDARY) });

    // Grid lines.
    float rect_width = PANE_WIDTH / current_level.width;
    float rect_height = PANE_HEIGHT / current_level.height;

    sf::Color grid_line_color = sf::Color(0, 255, 0, 75);

    for (int col = 1; col < current_level.width; ++col)
    {
        auto v_arr = sf::VertexArray(sf::LineStrip, 2);
        auto x_pos = WINDOW_LEFT_BOUNDARY + (rect_width * col);

        v_arr[0].position = sf::Vector2f(x_pos, WINDOW_TOP_BOUNDARY);
        v_arr[0].color = grid_line_color;

        v_arr[1].position = sf::Vector2f(x_pos, WINDOW_BOTTOM_BOUNDARY);
        v_arr[1].color = grid_line_color;

        grid_lines.push_back(v_arr);
    }

    for (int row = 1; row < current_level.height; ++row)
    {
        auto v_arr = sf::VertexArray(sf::LineStrip, 2);
        auto y_pos = WINDOW_TOP_BOUNDARY + (rect_height * row);

        v_arr[0].position = sf::Vector2f(WINDOW_LEFT_BOUNDARY, y_pos);
        v_arr[0].color = grid_line_color;

        v_arr[1].position = sf::Vector2f(WINDOW_RIGHT_BOUNDARY, y_pos);
        v_arr[1].color = grid_line_color;

        grid_lines.push_back(v_arr);
    }
}


void Simulation::handleContextSelection()
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


void Simulation::evaluateContextChange(const sf::Keyboard::Key& _key)
{
    if (!gd->input->getKeyDown(_key))
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


void Simulation::processContext()
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


void Simulation::processNavContext()
{
    if (gd->input->getMouseButtonDown(sf::Mouse::Left))
    {
        auto mouse_pos = gd->input->getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
        {
            return;
        }

        int tile_index = JHelper::posToTileIndex(mouse_pos, current_level);
        nav_manager->toggleTileWalkable(tile_index);
        std::cout << "Math tile index: " << tile_index << std::endl;
    }
}


void Simulation::processGameContext()
{
    if (gd->input->getMouseButtonDown(sf::Mouse::Left))
    {
        auto mouse_pos = gd->input->getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
        {
            return;
        }

        int tile_index = JHelper::posToTileIndex(mouse_pos, current_level);
        tower_manager->buildTowerOnTile(tile_index);
        std::cout << "Math tile index: " << tile_index << std::endl;
    }
}


void Simulation::processHeatmapContext()
{
    if (painting)
    {
        auto mouse_pos = gd->input->getMousePos();
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


std::string Simulation::contextToString(const ContextType& _context)
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
            return str + "Game";
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


void Simulation::updateContextDisplay()
{
    context_display.setString(contextToString(current_context));
    JHelper::centerSFOrigin(context_display);
}
