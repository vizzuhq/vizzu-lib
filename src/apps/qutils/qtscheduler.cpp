#include "qtscheduler.h"

#include <QTimer>

using namespace std::chrono;

void QtScheduler::schedule(const GUI::Scheduler::Task &task,
    steady_clock::time_point time)
{
	auto actTime = steady_clock::now();
	auto duration = time - actTime;
	int msecs = 0;
	if (time > actTime)
		msecs = duration_cast<milliseconds>(duration).count();
	QTimer::singleShot(msecs,
	    [=]()
	    {
		    task();
	    });
}
