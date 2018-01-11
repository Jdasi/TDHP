#include "Scheduler.h"
#include "JTime.h"


Scheduler::Scheduler()
    : scheduled_tasks()
    , total_time(0)
{
}


// Increments the scheduler's time record and processes its scheduled tasks.
void Scheduler::update()
{
    total_time += JTime::getDeltaTime();

    executeScheduledTasks();
}


// Calls the passed function after the specified time in seconds.
void Scheduler::invoke(const std::function<void()>& _method, const double _time,
    const std::string& _name)
{
    scheduled_tasks.emplace_back(_method, total_time + _time, _name);
}


void Scheduler::invokeRepeating(const std::function<void()>& _method,
    const double _start, const double _interval, const std::string& _name)
{
    scheduled_tasks.emplace_back(_method, total_time + _start, _interval, _name);
}


// Cancels any invoke with the passed name.
void Scheduler::cancelInvoke(const std::string& _name)
{
    scheduled_tasks.remove_if([_name](auto& _task) { return _task.name == _name; });
}


// Cancels all invokes on this scheduler.
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
