#pragma once

class InputHandler;
class AssetManager;
class NavManager;

struct GameData
{
    GameData()
        : input(nullptr)
        , asset_manager(nullptr)
        , exit(false)
    {
    }

    InputHandler* input;
    AssetManager* asset_manager;

    bool exit;
};
