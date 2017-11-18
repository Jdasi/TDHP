#include <string>

#include "GameManager.h"
#include "GameData.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "Constants.h"
#include "JHelper.h"


GameManager::GameManager(GameData* _gd)
    : gd(_gd)
    , current_context(ContextType::NAV)
    , painting(false)
{
    init();
}


void GameManager::tick()
{
    painting = gd->input->getMouseButton(sf::Mouse::Left);

    handleContextSelection();
    processContext();
}


void GameManager::draw(sf::RenderWindow& _window)
{
    _window.draw(context_display);
}


void GameManager::init()
{
    gd->nav_manager->createHeatMap(sf::Color::Red, 200, 15);
    gd->nav_manager->createHeatMap(sf::Color::Green, 200, 15);

    sf::Font* default_font = gd->asset_manager->loadFont(DEFAULT_FONT);

    context_display.setFont(*default_font);
    context_display.setCharacterSize(16);
    context_display.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 16);

    updateContextDisplay();
}


void GameManager::handleContextSelection()
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


void GameManager::evaluateContextChange(const sf::Keyboard::Key& _key)
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


void GameManager::processContext()
{
    switch (current_context)
    {
        case NAV:
        {
            if (gd->input->getMouseButtonDown(sf::Mouse::Left))
            {
                gd->nav_manager->toggleTileWalkable(gd->input->getMousePos());
            }
        } break;

        case GAME:
        {

        } break;

        case HEATMAP_0: case HEATMAP_1: case HEATMAP_2: case HEATMAP_3:
        case HEATMAP_4: case HEATMAP_5: case HEATMAP_6: case HEATMAP_7:
        {
            if (painting)
            {
                // Only affect the current selected heatmap.
                int heatmap_index = current_context - HEATMAP_0;
                gd->nav_manager->paintOnHeatMap(heatmap_index, gd->input->getMousePos(), 3);
            }
        } break;

        default: {}
    }
}


std::string GameManager::contextToString(const ContextType& _context)
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


void GameManager::updateContextDisplay()
{
    context_display.setString(contextToString(current_context));
    JHelper::centerSFOrigin(context_display);
}
