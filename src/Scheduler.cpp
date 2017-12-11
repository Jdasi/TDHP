#include "Scheduler.h"

Scheduler::Scheduler()
    : scheduled_tasks()
    , timer()
    , total_time(0)
{
}



// Ticks the Scheduler's own timer for the purposes of invoking scheduled functions.
void Scheduler::update()
{
    total_time += timer.getTimeDifference();
    timer.reset();

    executeScheduledTasks();
}



// Calls the passed function after the specified time in seconds.
void Scheduler::invoke(const std::function<void()>& _method, const double _time)
{
    scheduled_tasks.emplace_back(_method, total_time + _time);
}


void Scheduler::invokeRepeating(const std::function<void()>& _method,
    const double _start, const double _interval)
{
    scheduled_tasks.emplace_back(_method, total_time + _start, _interval);
}


void Scheduler::cancelInvokes()
{
    scheduled_tasks.clear();
}


/* Iterates through the list of scheduled tasks and executes them if their
 * scheduled time is exceeded. An executed task is then removed from the list,
 * but the iterator keeps going to the end of the list.
 */
void Scheduler::executeScheduledTasks()
{
    auto itr = scheduled_tasks.begin();
    while (itr != scheduled_tasks.end())
    {
        if (total_time >= itr->timeout)
        {
            if (itr->repeating)
            {
                itr->method();
                itr->timeout = total_time + itr->interval;
            }
            else
            {
                itr->method();

                itr = scheduled_tasks.erase(itr);
                if (itr == scheduled_tasks.end())
                {
                    break;
                }
            }
        }

        ++itr;
    }
}