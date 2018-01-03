#pragma once

enum HeatmapFlag
{
    NONE = 0,
    BLOOD = 1 << 0,
    ACID = 1 << 1,
    SMOKE = 1 << 2,

    ALL = BLOOD | ACID | SMOKE
};
