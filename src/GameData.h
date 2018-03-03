#pragma once

#include <string>

class InputHandler;
class AssetManager;
class GameAudio;
struct LevelData;

struct GameData
{
    GameData(InputHandler& _input, AssetManager& _assets, GameAudio& _audio)
        : input(_input)
        , assets(_assets)
        , audio(_audio)
        , selected_level_data(nullptr)
        , exit(false)
    {
    }

    InputHandler& input;
    AssetManager& assets;
    GameAudio& audio;

    LevelData* selected_level_data;

    bool exit;
};
