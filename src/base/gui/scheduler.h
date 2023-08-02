#ifndef BASE_GUI_SCHEDULER_H
#define BASE_GUI_SCHEDULER_H

#include <chrono>
#include <functional>
#include <map>
#include <memory>

namespace GUI
{

struct Scheduler
{
	using Task = std::function<void ()>;

	virtual ~Scheduler() = default;

	virtual void schedule(const Task &task,
	    std::chrono::steady_clock::time_point time) = 0;
};

using SchedulerPtr = std::shared_ptr<Scheduler>;

class TaskQueue : public Scheduler
{
public:
	void schedule(const Task &task,
	    std::chrono::steady_clock::time_point time) override;
	void poll();

private:
	std::multimap<std::chrono::steady_clock::time_point, Task> tasks;
};

}

#endif
