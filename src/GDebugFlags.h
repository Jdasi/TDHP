#pragma once

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Static struct which holds debug flags, for use with rendering
or logic systems that require different behaviours in Release
or Debug configurations.

---------------------------------------------------------*/
struct GDebugFlags final
{
    GDebugFlags() = delete;

    static bool draw_debug_controls;
    static bool draw_game;
    static bool draw_heatmaps;
    static bool draw_paths;
};
