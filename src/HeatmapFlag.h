#pragma once

enum HeatmapFlag
{
    // A heatmap should never be assigned the NONE flag.

    NONE = 0,

    // Assign one of these flags to a heatmap to identify its purpose.

    LASER_DEATHS = 1 << 0,
    BULLET_DEATHS = 1 << 1,
    TURRET_SHOTS = 1 << 2,
    DEBUG = 1 << 3,

    // These are used to identify multiple heatmaps, not individual ones.

    TURRETS = LASER_DEATHS | BULLET_DEATHS | TURRET_SHOTS,
    ALL = LASER_DEATHS | BULLET_DEATHS | TURRET_SHOTS | DEBUG
};
