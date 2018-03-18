#pragma once
#include <list>

#include "ScheduledTask.h"
#include "SimpleTimer.h"

/*-------------------------------------------------------
:: Author: Joe da Silva
:: Date: 18/03/2018

Class for queueing tasks to be executed at a later time.

The Scheduler's update method must be called to ensure the
scheduled tasks are executed.

This class uses the JTime static class to determine the activation
timing of scheduled tasks.
    - TimeUsageType::SCALED represents simulated game time.
    - TimeUsageType::UNSCALED represents real world time.

Scheduled tasks can be given names, allowing them to be cancelled
individually as they become obsolete.

---------------------------------------------------------*/
class Scheduler final
{
public:
    enum TimeUsageType
    {
        SCALED,
        UNSCALED
    };

    Scheduler(const TimeUsageType& _time_usage_type = TimeUsageType::SCALED);
    ~Scheduler() = default;

    void setTimeUsageType(const TimeUsageType& _type);
    void update();

    void invoke(const std::function<void()>& _method, const double _time,
        const std::string& _name = "");

    void invokeRepeating(const std::function<void()> & _method,
        const double _start, const double _interval,
        const std::string& _name = "");

    void cancelInvoke(const std::string& _name);
    void cancelInvokes();

private:
    void executeScheduledTasks();

    std::list<ScheduledTask> scheduled_tasks;
    double total_time;

    TimeUsageType time_usage_type;

};
