#ifndef ANIM_OPTIONS
#define ANIM_OPTIONS

#include "base/anim/time.h"
#include "base/anim/easing.h"

namespace Anim
{

class Options
{
public:
	Easing easing;
	Duration duration;
	Duration delay;

	Options() {}

	Options(Duration duration, Duration delay = Duration(0), Easing easing = Easing()) :
		easing(std::move(easing)),
		duration(duration),
		delay(delay)
	{}

	bool started(Duration time) const
	{
		return time >= start();
	}

	bool ended(Duration time) const
	{
		return time >= end();
	}

	Duration start() const
	{
		return delay;
	}

	Duration end() const {
		return delay + duration;
	}

	double getFactor(Duration time) const
	{
		if (!started(time)) return 0;
		if (ended(time)) return 1;
		return easing((time - delay) / duration);
	}
};

}

#endif
