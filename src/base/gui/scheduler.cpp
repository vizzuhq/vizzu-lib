
#include "scheduler.h"

void GUI::TaskQueue::schedule(const Scheduler::Task &task,
    std::chrono::steady_clock::time_point time)
{
	tasks.insert({time, task});
}

void GUI::TaskQueue::poll()
{
	auto now = std::chrono::steady_clock::now();
	auto oldestIt = tasks.begin();
	while (oldestIt != tasks.end() && oldestIt->first <= now) {
		oldestIt->second();
		tasks.erase(oldestIt);
		oldestIt = tasks.begin();
	}
}
