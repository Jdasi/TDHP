#pragma once

enum HeatmapFlag
{
    NONE = 0,
    LASER_DEATHS = 1 << 0,
    BULLET_DEATHS = 1 << 1,
    TURRET_SHOTS = 1 << 2,
    DEBUG = 1 << 3,

    TURRETS = LASER_DEATHS | BULLET_DEATHS | TURRET_SHOTS,
    ALL = LASER_DEATHS | BULLET_DEATHS | TURRET_SHOTS | DEBUG
};
