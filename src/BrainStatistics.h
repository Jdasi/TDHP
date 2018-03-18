#pragma once

#include <string>

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct to record statistics about BrainAction usage.

---------------------------------------------------------*/
struct BrainStatistics final
{
    std::string level_name      = "";
    float session_duration      = 0;

    int decision_points         = 0;
    int no_action_times         = 0;

    int fast_swarm_times        = 0;
    int strong_swarm_times      = 0;
    int basic_swarm_times       = 0;

    int hb_fast_times           = 0;
    int hb_strong_times         = 0;
    int hb_basic_times          = 0;
    int hb_all_times            = 0;

    int sb_fast_times           = 0;
    int sb_strong_times         = 0;
    int sb_basic_times          = 0;
    int sb_all_times            = 0;

    int smoke_times             = 0;

    int completed_paths         = 0;
};
