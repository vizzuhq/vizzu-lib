#ifndef QUTILS_QTSCHEDULER_H
#define QUTILS_QTSCHEDULER_H

#include "base/gui/scheduler.h"

class QtScheduler : public GUI::Scheduler
{
public:
	QtScheduler() = default;

	void schedule(const GUI::Scheduler::Task &task,
	    std::chrono::steady_clock::time_point time) override;
};

#endif
