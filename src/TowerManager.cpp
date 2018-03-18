#include <SFML/Window/Mouse.hpp>

#include "TowerManager.h"
#include "AssetManager.h"
#include "GameAudio.h"
#include "NavManager.h"
#include "EnemyDirector.h"
#include "JHelper.h"
#include "Level.h"
#include "FileIO.h"
#include "GameData.h"


TowerManager::TowerManager(GameData& _game_data, NavManager& _nav_manager,
    HeatmapManager& _heatmap_manager, EnemyDirector& _enemy_director, Level& _level)
    : gd(_game_data)
    , nav_manager(_nav_manager)
    , heatmap_manager(_heatmap_manager)
    , enemy_director(_enemy_director)
    , level(_level)
    , projectile_manager(_game_data, _heatmap_manager, _enemy_director, _level)
    , enemy_destination(enemy_director.getEnemyDestination())
    , slots_used(0)
{
    initTowers();
    initTowerSlots();

    scheduler.invokeRepeating([this]()
    {
        updateTowerTargets();
    }, 0, 0.05f);
}


void TowerManager::tick()
{
    projectile_manager.tick();
    scheduler.update();

    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        tower.tick();
    }
}


void TowerManager::draw(sf::RenderWindow& _window)
{
    projectile_manager.draw(_window);

    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        tower.draw(_window);
    }

    for (auto& slot : tower_slots)
    {
        _window.draw(slot);
    }

    _window.draw(lbl_slots);
}


void TowerManager::removeTower(const int _tile_index)
{
    if (!JHelper::validIndex(_tile_index, level.getProduct()))
        return;

    if (towerExists(_tile_index) && nav_manager.isNodeWalkable(_tile_index))
    {
        deconstructTower(_tile_index);
    }
}


void TowerManager::toggleTower(const int _tile_index, int _click_type)
{
    if (!JHelper::validIndex(_tile_index, level.getProduct()))
        return;

    if (towerExists(_tile_index))
    {
        deconstructTower(_tile_index);
    }
    else
    {
        if (!nav_manager.isNodeWalkable(_tile_index))
        {
            constructTower(_tile_index, clickTypeToTowerSlug(_click_type));
        }
        else
        {
            // Invalid tile picked for construction.
            gd.audio.playSound(TOWER_ERROR_SOUND);
        }
    }
}


void TowerManager::initTowers()
{
    tower_types = FileIO::loadTowerTypes(gd.assets);

    for (auto& tower : towers)
    {
        tower.init(projectile_manager, enemy_destination);
    }
}


void TowerManager::initTowerSlots()
{
    // Try to keep the tower slots centered around the height of the window.
    auto* tex = gd.assets.loadTexture(SLOT_EMPTY_TEXTURE);
    float start_y = (WINDOW_HEIGHT * 0.5f) - (MAX_TOWERS * (tex->getSize().y / 2));

    for (int i = 0; i < MAX_TOWERS; ++i)
    {
        auto& slot = tower_slots[i];

        // Line the slots up beneath each other.
        slot.setTexture(*tex);
        slot.setPosition(WINDOW_WIDTH * 0.95f, start_y + (i * slot.getGlobalBounds().height));

        JHelper::centerSFOrigin(slot);
    }

    // Slots Used Label.
    lbl_slots.setFont(*gd.assets.loadFont(DEFAULT_FONT));
    lbl_slots.setCharacterSize(14);
    lbl_slots.setStyle(sf::Text::Bold);
    lbl_slots.setFillColor(sf::Color::White);
    lbl_slots.setString("Towers\n  Used:");

    auto top_pos = tower_slots[0].getPosition();
    lbl_slots.setPosition(top_pos - sf::Vector2f(0, static_cast<float>(tex->getSize().y)));

    JHelper::centerSFOrigin(lbl_slots);
}


void TowerManager::updateTowerTargets()
{
    for (auto& tower : towers)
    {
        if (!tower.isAlive())
            continue;

        auto& tower_pos = tower.getPosition();
        auto& nearby_enemies = enemy_director.getEnemiesNearPosSqr(
            tower_pos, tower.getEngageRadiusSqr());

        tower.updateNearbyEnemies(nearby_enemies);
    }
}


void TowerManager::constructTower(const int _tile_index, const std::string& _tower_slug)
{
    for (auto& tower : towers)
    {
        if (tower.isAlive())
            continue;

        tower.setTileIndex(_tile_index);
        tower.setPosition(JHelper::tileIndexToTileCenter(_tile_index, level));
        tower.setType(tower_types.at(_tower_slug));

        tower.spawn();
        gd.audio.playSound(TOWER_PLACE_SOUND);

        increaseSlotsUsed();

        return;
    }

    // All tower slots used.
    gd.audio.playSound(TOWER_ERROR_SOUND);
}


void TowerManager::deconstructTower(const int _tile_index)
{
    for (auto& tower : towers)
    {
        if (tower.getTileIndex() != _tile_index)
            continue;

        tower.killQuiet();
        tower.setTileIndex(-1);

        gd.audio.playSound(TOWER_DESTROY_SOUND);

        decreaseSlotsUsed();

        return;
    }
}


bool TowerManager::towerExists(const int _tile_index) const
{
    for (auto& tower : towers)
    {
        if (tower.getTileIndex() == _tile_index)
            return true;
    }

    return false;
}


std::string TowerManager::clickTypeToTowerSlug(const int _click_type)
{
    switch (_click_type)
    {
        case sf::Mouse::Left: return LASER_TOWER_SLUG;
        case sf::Mouse::Right: return BULLET_TOWER_SLUG;

        default: return LASER_TOWER_SLUG;
    }
}


void TowerManager::increaseSlotsUsed()
{
    auto* tex = gd.assets.loadTexture(SLOT_FILLED_TEXTURE);
    tower_slots[slots_used].setTexture(*tex);

    ++slots_used;
}


void TowerManager::decreaseSlotsUsed()
{
    auto* tex = gd.assets.loadTexture(SLOT_EMPTY_TEXTURE);
    tower_slots[slots_used - 1].setTexture(*tex);

    --slots_used;
}
