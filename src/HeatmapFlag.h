#pragma once

enum HeatmapFlag
{
    // A heatmap should never be assigned the NONE flag.

    NONE = 0,

    // Assign one of the following flags to a heatmap to identify its purpose.

    LASER_DEATHS = 1 << 0,
    BULLET_DEATHS = 1 << 1,
    SMOKE = 1 << 2,
    DEBUG = 1 << 3,

    // The following flags are used to identify multiple heatmaps, not individual ones.

    DIRECTOR = LASER_DEATHS | BULLET_DEATHS | SMOKE,
    ALL = LASER_DEATHS | BULLET_DEATHS | SMOKE | DEBUG
};
