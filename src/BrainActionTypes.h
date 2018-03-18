#pragma once

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Enum identifiers of all the available action types, and precondition
types that actions can use to focus their application.

---------------------------------------------------------*/
namespace Brain
{

enum class PreconditionType
{
    BULLET_INTENSITY_OVER_THRESHOLD,
    LASER_INTENSITY_OVER_THRESHOLD,
    FAST_ENEMIES_OVER_THRESHOLD,
    STRONG_ENEMIES_OVER_THRESHOLD,
    BASIC_ENEMIES_OVER_THRESHOLD,
    OVERALL_INTENSITY_OVER_THRESHOLD,
    ENEMY_CLOSE_TO_GOAL,
    BULLET_INTENSITY_GREATER,
    LASER_INTENSITY_GREATER,
    FAST_ENEMIES_GREATER,
    STRONG_ENEMIES_GREATER,
    BASIC_ENEMIES_GREATER
};

enum class ActionType
{
    SEND_FAST_SWARM,
    SEND_STRONG_SWARM,
    HEALTH_BOOST_ALL_ENEMIES,
    SPEED_BOOST_ALL_ENEMIES,
    HEALTH_BOOST_FAST_ENEMIES,
    HEALTH_BOOST_STRONG_ENEMIES,
    HEALTH_BOOST_BASIC_ENEMIES,
    SPEED_BOOST_FAST_ENEMIES,
    SPEED_BOOST_STRONG_ENEMIES,
    SPEED_BOOST_BASIC_ENEMIES,
    SMOKE_BOMB,
    NO_ACTION
};

} // namespace brain
