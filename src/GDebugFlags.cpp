#include "GDebugFlags.h"

#ifdef _DEBUG
    bool GDebugFlags::draw_debug_controls = true;
    bool GDebugFlags::draw_game = true;
    bool GDebugFlags::draw_heatmaps = true;
    bool GDebugFlags::draw_paths = true;
#else
    bool GDebugFlags::draw_debug_controls = false;
    bool GDebugFlags::draw_game = true;
    bool GDebugFlags::draw_heatmaps = true;
    bool GDebugFlags::draw_paths = false;
#endif
