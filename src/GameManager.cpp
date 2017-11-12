#include <SFML/Graphics.hpp>

#include "GameManager.h"
#include "GameData.h"
#include "InputHandler.h"
#include "AssetManager.h"
#include "NavManager.h"


GameManager::GameManager(GameData* _gd)
    : gd(_gd)
    , debug_heat_map(nullptr)
    , painting(false)
{
    debug_heat_map = gd->nav_manager->createHeatMap(sf::Color::Red, 200, 15);
}


void GameManager::tick()
{
    painting = gd->input->getMouseButton(sf::Mouse::Left);

    if (painting)
    {
        debug_heat_map->paint(gd->input->getMousePos(), 3);
    }

    if (gd->input->getMouseButtonDown(sf::Mouse::Right))
    {
        gd->nav_manager->toggleTileWalkable(gd->input->getMousePos());
    }
}


void GameManager::draw(sf::RenderWindow& _window)
{
    // Game stuff, eventually ..
}
