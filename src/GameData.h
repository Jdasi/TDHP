#pragma once

#include <string>

class InputHandler;
class AssetManager;
class GameAudio;
struct LevelData;

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct to hold references to core game systems used in many
different areas of the program.

---------------------------------------------------------*/
struct GameData final
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
