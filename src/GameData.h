#pragma once

class AssetManager;
class NavManager;

struct GameData
{
    GameData()
        : asset_manager(nullptr)
        , nav_manager(nullptr)
        , exit(false)
    {
    }

    AssetManager* asset_manager;
    NavManager* nav_manager;
    bool exit;
};
