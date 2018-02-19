#include <string>

#include "Game.h"
#include "GameData.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "JHelper.h"
#include "FileIO.h"
#include "GDebugFlags.h"
#include "GameAudio.h"


Game::Game(GameData& _gd)
    : gd(_gd)
    , current_level(_gd.level_name)
    , current_context(ContextType::GAME)
    , painting(false)
{
    init();
}


void Game::tick()
{
    // Systems.
    heatmap_manager->tick();
    enemy_director->tick();
    tower_manager->tick();
    score_manager->tick();

    // State.
    painting = gd.input.getMouseButton(sf::Mouse::Left);

    if (GDebugFlags::draw_debug_controls)
        handleContextSelection();

    processContext();

    if (!player_health->isAlive())
    {
        for (auto& listener : listeners)
        {
            listener->onGameOver();
        }

        gd.audio.playSound(GAME_OVER_SOUND);
    }
}


void Game::draw(sf::RenderWindow& _window)
{
    if (GDebugFlags::draw_game)
        _window.draw(border);

    if (GDebugFlags::draw_heatmaps)
        heatmap_manager->draw(_window);

    if (GDebugFlags::draw_game)
        current_level.draw(_window);

    for (auto& arr : grid_lines)
    {
        _window.draw(arr);
    }

    if (GDebugFlags::draw_game)
    {
        enemy_director->draw(_window);
        tower_manager->draw(_window);
        score_manager->draw(_window);
        player_health->draw(_window);
    }

    if (GDebugFlags::draw_debug_controls)
        _window.draw(context_display);
}


int Game::getScore() const
{
    return score_manager->getScore();
}


void Game::exportCurrentLevel() const
{
    FileIO::exportLevel(current_level);
}


void Game::init()
{
    initSystems();
    initObjects();

    parseCurrentLevel();
    enemy_director->updatePaths();
}


void Game::initSystems()
{
    TDSprite::init(current_level);

    heatmap_manager = std::make_unique<HeatmapManager>(current_level);

    nav_manager = std::make_unique<NavManager>(*heatmap_manager.get(),
        current_level);

    enemy_director = std::make_unique<EnemyDirector>(gd,
        *nav_manager.get(), *heatmap_manager.get(), current_level);

    tower_manager = std::make_unique<TowerManager>(gd,
        *nav_manager.get(), *heatmap_manager.get(), *enemy_director.get(),
        current_level);

    score_manager = std::make_unique<ScoreManager>(gd, current_level);
    enemy_director->addEnemyListener(score_manager.get());

    player_health = std::make_unique<PlayerHealth>(gd);
    enemy_director->addEnemyListener(player_health.get());
}


void Game::initObjects()
{
    sf::Font* default_font = gd.assets.loadFont(DEFAULT_FONT);

    // Debug context display.
    context_display.setFont(*default_font);
    context_display.setCharacterSize(12);
    context_display.setFillColor(sf::Color::Red);
    context_display.setPosition(WINDOW_WIDTH * 0.78f, 0);

    updateContextDisplay();

    initBorder();
    initGridLines();
}


void Game::initBorder()
{
    auto* texture = gd.assets.loadTexture(FLOOR_TEXTURE);

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
    for (int col = 0; col <= current_level.getSizeX(); ++col)
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
    for (int row = 0; row <= current_level.getSizeY(); ++row)
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


void Game::parseCurrentLevel()
{
    auto* texture = gd.assets.loadTexture(UNWALKABLE_TEXTURE);
    current_level.setUnwalkableTexture(texture);

    int size_x = current_level.getSizeX();
    int size_y = current_level.getSizeY();

    for (int row = 0; row < size_y; ++row)
    {
        for (int col = 0; col < size_x; ++col)
        {
            int index = JHelper::calculateIndex(col, row, size_x);
            auto tile_type = current_level.getTileType(index);

            switch (tile_type)
            {
                case Level::UNWALKABLE:
                {
                    nav_manager->toggleNodeWalkable(index);
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
    for (int i = sf::Keyboard::Num0; i <= sf::Keyboard::Num9; ++i)
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
    int selection = static_cast<int>(_key) - static_cast<int>(sf::Keyboard::Num1);

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

        current_level.updateTileType(index, nav_manager->isNodeWalkable(index) ?
            Level::WALKABLE : Level::UNWALKABLE);

        enemy_director->removeEnemySpawn(index);
        enemy_director->updatePaths();
        tower_manager->removeTower(index);
    }
    else if (gd.input.getMouseButtonDown(sf::Mouse::Right))
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
            return;

        // Clear prev destination from level data.
        int prev_index = enemy_director->getEnemyDestination().tile_index;
        current_level.updateTileType(prev_index, Level::WALKABLE);

        // Set new destination with enemy director and then level data.
        int index = JHelper::posToTileIndex(mouse_pos, current_level);
        enemy_director->setEnemyDestination(index);

        if (nav_manager->isNodeWalkable(index))
            current_level.updateTileType(index, Level::ENEMY_DESTINATION);
    }
    else if (gd.input.getMouseButtonDown(sf::Mouse::Middle))
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
            return;

        int index = JHelper::posToTileIndex(mouse_pos, current_level);
        if (!nav_manager->isNodeWalkable(index))
            nav_manager->toggleNodeWalkable(index);

        enemy_director->toggleEnemySpawn(index);

        current_level.updateTileType(index, enemy_director->spawnExists(index) ?
            Level::ENEMY_SPAWN : Level::WALKABLE);
    }
}


void Game::processGameContext()
{
    const int NO_CLICK = -1;
    int click_type = NO_CLICK; // Assume no click.
    
    if (gd.input.getMouseButtonDown(sf::Mouse::Left))
        click_type = sf::Mouse::Left;

    if (gd.input.getMouseButtonDown(sf::Mouse::Right))
        click_type = sf::Mouse::Right;

    if (click_type != NO_CLICK)
    {
        auto mouse_pos = gd.input.getMousePos();
        if (!JHelper::posInSimulationArea(mouse_pos))
        {
            return;
        }

        int index = JHelper::posToTileIndex(mouse_pos, current_level);
        tower_manager->toggleTower(index, click_type);
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
    context_display.setString("[" + contextToString(current_context) + "]");
}
