#pragma once

struct GDebugFlags
{
    GDebugFlags() = delete;

    static bool draw_debug_controls;
    static bool draw_game;
    static bool draw_heatmaps;
    static bool draw_paths;
};
