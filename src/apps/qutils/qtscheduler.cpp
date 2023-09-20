#include "qtscheduler.h"

#include <QTimer>

using std::chrono::milliseconds;
using std::chrono::steady_clock;

void QtScheduler::schedule(
    [[maybe_unused]] const GUI::Scheduler::Task &task,
    steady_clock::time_point time)
{
	auto actTime = steady_clock::now();
	auto duration = time - actTime;
	[[maybe_unused]] long msecs = 0;
	if (time > actTime)
		msecs = duration_cast<milliseconds>(duration).count();
#ifndef __clang_analyzer__
	return QTimer::singleShot(msecs, task);
#endif
}
