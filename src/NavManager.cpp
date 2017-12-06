#include "NavManager.h"
#include "JHelper.h"
#include "Level.h"
#include "Constants.h"


NavManager::NavManager(HeatmapManager& _heatmap_manager, Level& _level)
    : heatmap_manager(_heatmap_manager)
    , level(_level)
{
    nav_nodes.assign(_level.getProduct(), NavNode());
}


bool NavManager::isNodeWalkable(const int _index) const
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return false;

    return nav_nodes[_index].isWalkable();
}


void NavManager::setNodeWalkable(const int _index, const bool _walkable)
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return;

    auto tile_type = level.getLevelTileType(_index);
    if (tile_type != Level::WALKABLE && tile_type != Level::UNWALKABLE)
        return;

    auto& nav_node = nav_nodes[_index];
    nav_node.setWalkable(_walkable);

    level.getGrid().setTileColor(_index, isNodeWalkable(_index) ?
        WALKABLE_COLOR : UNWALKABLE_COLOR);
}


void NavManager::toggleNodeWalkable(const int _index)
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return;

    auto& nav_node = nav_nodes[_index];
    setNodeWalkable(_index, !nav_node.isWalkable());
}
