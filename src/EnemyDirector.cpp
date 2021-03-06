#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>

#include "EnemyDirector.h"
#include "AssetManager.h"
#include "NavManager.h"
#include "InputHandler.h"
#include "GameData.h"
#include "HeatmapManager.h"
#include "JHelper.h"
#include "JMath.h"
#include "Level.h"
#include "FileIO.h"
#include "TowerType.h"
#include "GDebugFlags.h"


EnemyDirector::EnemyDirector(GameData& _game_data, NavManager& _nav_manager,
    HeatmapManager& _heatmap_manager, Level& _level)
    : gd(_game_data)
    , nav_manager(_nav_manager)
    , heatmap_manager(_heatmap_manager)
    , level(_level)
    , enemy_manager(_game_data.assets, enemy_destination)
    , brain(_game_data, _heatmap_manager, enemy_manager, enemy_spawns, _level)
{
    enemy_spawns.reserve(MAX_ENEMY_SPAWNS);

    init();
}


void EnemyDirector::tick()
{
    scheduler.update();
    brain.tick();

    energy_bar.updateValuePercentage(brain.getEnergyPercentage());

    if (GDebugFlags::draw_debug_controls)
        handleDebugCommands();

    for (auto& spawn : enemy_spawns)
    {
        spawn->tick();
    }

    enemy_manager.tick();
}


void EnemyDirector::draw(sf::RenderWindow& _window)
{
    for (auto& spawn : enemy_spawns)
    {
        spawn->draw(_window);
    }

    brain.draw(_window);

    destination_marker.draw(_window);
    enemy_manager.draw(_window);

    energy_bar.draw(_window);
    _window.draw(lbl_energy);
}


bool EnemyDirector::spawnExists(const int _tile_index)
{
    if (!JHelper::validIndex(_tile_index, level.getProduct()))
        return false;

    for (auto& spawn : enemy_spawns)
    {
        if (spawn->getTileIndex() != _tile_index)
            continue;

        return true;
    }

    return false;
}


bool EnemyDirector::addEnemySpawn(const int _tile_index)
{
    if (enemy_spawns.size() >= MAX_ENEMY_SPAWNS ||
        !nav_manager.isNodeWalkable(_tile_index))
    {
        return false;
    }

    auto spawn = std::make_unique<EnemySpawn>(nav_manager, level, _tile_index,
        enemy_destination, enemy_manager);

    auto* texture = gd.assets.loadTexture(SPAWN_TEXTURE);
    spawn->setMarkerTexture(texture);

    spawn->updatePaths();
    enemy_spawns.push_back(std::move(spawn));

    return true;
}


bool EnemyDirector::removeEnemySpawn(const int _tile_index)
{
    auto object = std::find_if(enemy_spawns.begin(), enemy_spawns.end(),
            [&](auto& _obj) { return _obj->getTileIndex() == _tile_index; });

    if (object == enemy_spawns.end())
        return false;

    enemy_spawns.erase(std::remove(enemy_spawns.begin(), enemy_spawns.end(), *object));

    return true;
}


void EnemyDirector::toggleEnemySpawn(const int _tile_index)
{
    if (removeEnemySpawn(_tile_index))
        return;

    addEnemySpawn(_tile_index);
}


void EnemyDirector::updatePaths()
{
    for (auto& spawn : enemy_spawns)
    {
        spawn->updatePaths();
    }
}


void EnemyDirector::addEnemyListener(EnemyListener* _listener)
{
    enemy_manager.addEnemyListener(_listener);
}


Waypoint& EnemyDirector::getEnemyDestination()
{
    return enemy_destination;
}


void EnemyDirector::setEnemyDestination(const int _tile_index)
{
    if (!JHelper::validIndex(_tile_index, level.getProduct()))
        return;

    enemy_destination = level.createWaypoint(_tile_index);
    destination_marker.setPosition(enemy_destination.pos);

    updatePaths();
}


std::vector<Enemy*> EnemyDirector::getEnemiesNearPosSqr(const sf::Vector2f& _pos,
    const float _radius_sqr)
{
    return enemy_manager.getEnemiesNearPosSqr(_pos, _radius_sqr);
}


bool EnemyDirector::damageEnemyAtPos(const sf::Vector2f& _pos, TowerType* _attacker_type)
{
    return enemy_manager.damageEnemyAtPos(_pos, _attacker_type);
}


bool EnemyDirector::killEnemyAtPos(const sf::Vector2f& _pos, TowerType* _killer_type)
{
    return enemy_manager.killEnemyAtPos(_pos, _killer_type);
}


void EnemyDirector::init()
{
    initVisualisations();
    initRegularSpawning();
}


void EnemyDirector::initVisualisations()
{
    // Energy bar label.
    lbl_energy.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_energy.setCharacterSize(14);
    lbl_energy.setStyle(sf::Text::Bold);
    lbl_energy.setFillColor(sf::Color::White);
    lbl_energy.setPosition({ WINDOW_WIDTH * 0.15f, WINDOW_HEIGHT * 0.04f });
    lbl_energy.setString("Director Energy:");

    JHelper::centerSFOrigin(lbl_energy);

    // Energy bar.
    energy_bar.configure({ PANE_WIDTH * 0.825f, WINDOW_HEIGHT * 0.025f }, 0, sf::Color::Blue);
    energy_bar.updatePosition({ WINDOW_WIDTH * 0.56f, WINDOW_HEIGHT * 0.04f });

    // Destination marker.
    auto* texture = gd.assets.loadTexture(DESTINATION_TEXTURE);
    destination_marker.setTexture(texture);
}


void EnemyDirector::initRegularSpawning()
{
    scheduler.invokeRepeating([this]()
    {
        if (enemy_spawns.empty())
            return;

        EnemyType* regular_type = enemy_manager.getBasicType();
        EnemySpawn* spawn = enemy_spawns[rand() % enemy_spawns.size()].get();

        if (spawn->enemiesInQueue())
            return;

        spawn->spawnEnemy(regular_type);
    }, DIRECTOR_SPAWN_DELAY, DIRECTOR_SPAWN_DELAY);
}


// Commands that an only be triggered by the user in debug mode.
void EnemyDirector::handleDebugCommands()
{
    if (gd.input.getKeyDown(sf::Keyboard::V))
    {
        if (enemy_spawns.empty())
            return;

        // Spawn a random enemy.
        EnemyType* random_type = enemy_manager.getRandomType();
        enemy_spawns[rand() % enemy_spawns.size()]->spawnEnemy(random_type);
    }
    else if (gd.input.getKeyDown(sf::Keyboard::Z))
    {
        if (enemy_spawns.empty())
            return;

        // Spawn fastest enemy.
        EnemyType* random_type = enemy_manager.getFastestType();
        enemy_spawns[rand() % enemy_spawns.size()]->spawnEnemy(random_type);
    }
    else if (gd.input.getKeyDown(sf::Keyboard::X))
    {
        if (enemy_spawns.empty())
            return;

        // Spawn basic enemy.
        EnemyType* random_type = enemy_manager.getBasicType();
        enemy_spawns[rand() % enemy_spawns.size()]->spawnEnemy(random_type);
    }
    else if (gd.input.getKeyDown(sf::Keyboard::C))
    {
        if (enemy_spawns.empty())
            return;

        // Spawn strongest enemy.
        EnemyType* random_type = enemy_manager.getStrongestType();
        enemy_spawns[rand() % enemy_spawns.size()]->spawnEnemy(random_type);
    }
    else if (gd.input.getKeyDown(sf::Keyboard::S))
    {
        // Boost all enemy speed.
        enemy_manager.boostEnemySpeed(2, 3);
    }
    else if (gd.input.getKeyDown(sf::Keyboard::H))
    {
        // Boost all enemy health.
        enemy_manager.boostEnemyHealth(2, 3);
    }
    else if (gd.input.getKeyDown(sf::Keyboard::M))
    {
        // Queue N of a random type from a single spawn point.
        EnemyType* random_type = enemy_manager.getRandomType();
        EnemySpawn* spawn = enemy_spawns[rand() % enemy_spawns.size()].get();
        LevelPath path = spawn->getPath();

        float spawn_delay = 2 - (random_type->speed * 0.02f);
        spawn_delay = JMath::clampf(spawn_delay, 0.5f, 2);

        for (int i = 0; i < 3; ++i)
        {
            spawn->queueEnemy(random_type, i * spawn_delay, path);
        }
    }
    else if (gd.input.getKeyDown(sf::Keyboard::N))
    {
        // Clear all spawn queues.
        for (auto& spawn : enemy_spawns)
        {
            spawn->clearSpawnQueue();
        }
    }
}
