#include "NavManager.h"
#include "JHelper.h"


NavManager::NavManager(HeatmapManager& _heatmap_manager, const int _level_width,
    const int _level_height)
    : heatmap_manager(_heatmap_manager)
{
    nav_nodes.assign(_level_width * _level_height, NavNode());
}


bool NavManager::isTileWalkable(const int _index) const
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return false;

    return nav_nodes[_index].isWalkable();
}


void NavManager::toggleTileWalkable(const int _index)
{
    if (!JHelper::validIndex(_index, nav_nodes.size()))
        return;

    auto& nav_node = nav_nodes[_index];
    nav_node.toggleWalkable();
}
