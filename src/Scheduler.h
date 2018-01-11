#pragma once
#include <list>

#include "ScheduledTask.h"
#include "SimpleTimer.h"

/* Class for scheduling tasks that should take place after a specified
 * amount of time.
 *
 * The Scheduler's update method must be called in order to ensure
 * prompt activation of scheduled tasks.
 */
class Scheduler
{
public:
    Scheduler();
    ~Scheduler() = default;

    void update();

    void invoke(const std::function<void()>& _method, const double _time);
    void invokeRepeating(const std::function<void()> & _method,
        const double _start, const double _interval);

    void cancelInvokes();

private:
    void executeScheduledTasks();

    std::list<ScheduledTask> scheduled_tasks;
    double total_time;

};
