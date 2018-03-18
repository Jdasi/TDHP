#pragma once

#include <functional>

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Struct containing information related to a task to be scheduled
and later executed by a Scheduler.

Providing a name for the task is optional, but doing so allows
for the task to be cancelled by name if it becomes obsolete
(E.g. if a repeating task is no longer needed).

---------------------------------------------------------*/
struct ScheduledTask
{
    ScheduledTask(const std::function<void()>& _method,
        const double _timeout, const double _interval,
        const std::string& _name)
        : method(_method)
        , timeout(_timeout)
        , interval(_interval)
        , repeating(true)
        , name(_name)
    {
    }

    ScheduledTask(const std::function<void()>& _method, const double _timeout,
        const std::string& _name)
        : method(_method)
        , timeout(_timeout)
        , interval(0)
        , repeating(false)
        , name(_name)
    {
    }

    std::function<void()> method;
    double timeout;
    double interval;
    bool repeating;
    std::string name;
};
