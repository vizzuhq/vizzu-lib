#ifndef GUI_SCHEDULER
#define GUI_SCHEDULER

#include <chrono>
#include <functional>
#include <map>
#include <memory>

namespace GUI
{

struct Scheduler
{
	typedef std::function<void(void)> Task;

	virtual ~Scheduler() {}

	virtual void schedule(const Task &task,
	    std::chrono::steady_clock::time_point time) = 0;
};

typedef std::shared_ptr<Scheduler> SchedulerPtr;

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
