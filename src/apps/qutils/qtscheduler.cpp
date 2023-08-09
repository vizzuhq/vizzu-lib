#include "qtscheduler.h"

#include <QTimer>

using namespace std::chrono;

void QtScheduler::schedule(
    [[maybe_unused]] const GUI::Scheduler::Task &task,
    steady_clock::time_point time)
{
	auto actTime = steady_clock::now();
	auto duration = time - actTime;
	[[maybe_unused]] int msecs = 0;
	if (time > actTime)
		msecs = duration_cast<milliseconds>(duration).count();
#ifndef __clang_analyzer__
	return QTimer::singleShot(msecs, task);
#endif
}
