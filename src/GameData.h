#pragma once

#include <string>

class InputHandler;
class AssetManager;
class GameAudio;

struct GameData
{
    GameData(InputHandler& _input, AssetManager& _assets, GameAudio& _audio)
        : input(_input)
        , assets(_assets)
        , audio(_audio)
        , exit(false)
    {
    }

    InputHandler& input;
    AssetManager& assets;
    GameAudio& audio;

    std::string level_name;

    bool exit;
};
