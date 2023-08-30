#ifndef ANIM_OPTIONS
#define ANIM_OPTIONS

#include "duration.h"
#include "easing.h"

namespace Anim
{

class Options
{
public:
	Easing easing;
	Duration duration;
	Duration delay;

	Options() = default;

	Options(const Options&) = default;
	Options(Options&&) noexcept = default;

	explicit Options(Duration duration,
	    Duration delay = Duration(0),
	    Easing easing = Easing()) :
	    easing(std::move(easing)),
	    duration(duration),
	    delay(delay)
	{}

	[[nodiscard]] bool started(Duration time) const
	{
		return time >= start();
	}

	[[nodiscard]] bool ended(Duration time) const
	{
		return time >= end();
	}

	[[nodiscard]] Duration start() const { return delay; }

	[[nodiscard]] Duration end() const { return delay + duration; }

	[[nodiscard]] double getFactor(Duration time) const
	{
		if (!started(time)) return 0;
		if (ended(time)) return 1;
		return easing((time - delay) / duration);
	}
};

}

#endif
