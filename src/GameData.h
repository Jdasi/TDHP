#pragma once

class InputHandler;
class AssetManager;
class NavManager;

struct GameData
{
    GameData(InputHandler& _input_manager, AssetManager& _asset_manager)
        : input(_input_manager)
        , asset_manager(_asset_manager)
        , exit(false)
    {
    }

    InputHandler& input;
    AssetManager& asset_manager;

    bool exit;
};
