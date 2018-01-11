#pragma once

#include <functional>

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
