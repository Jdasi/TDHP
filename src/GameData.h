#pragma once

class InputHandler;
class AssetManager;
class NavManager;

struct GameData
{
    GameData()
        : input(nullptr)
        , asset_manager(nullptr)
        , nav_manager(nullptr)
        , exit(false)
    {
    }

    InputHandler* input;
    AssetManager* asset_manager;
    NavManager* nav_manager;

    bool exit;
};
