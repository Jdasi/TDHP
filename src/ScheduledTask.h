#pragma once
#include <functional>

struct ScheduledTask
{
    ScheduledTask(const std::function<void()>& _method,
        const double _timeout, const double _interval)
        : method(_method)
        , timeout(_timeout)
        , interval(_interval)
        , repeating(true)
    {
    }

    ScheduledTask(const std::function<void()>& _method, const double _timeout)
        : method(_method)
        , timeout(_timeout)
        , interval(0)
        , repeating(false)
    {
    }

    std::function<void()> method;
    double timeout;
    double interval;
    bool repeating;
};
