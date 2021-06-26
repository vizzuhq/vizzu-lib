#ifndef ANIM_CONTROL
#define ANIM_CONTROL

#include <functional>

#include "base/anim/controllable.h"
#include "base/refl/enum.h"

namespace Anim
{

class Control
{
public:
	class Enum(PlayState)( paused, running );
	class Enum(Direction)( normal, reverse );

	typedef std::function<void()> Event;

	Control(Controllable &controlled);
	void update(const TimePoint &time);

	void seek(const std::string &value);
	void seekProgress(double value);
	void seekTime(Duration pos);
	void pause() { playState = PlayState::paused; }
	void play() { playState = PlayState::running; }
	void stop();

	void reverse() {
		direction = direction == Direction::normal
		              ? Direction::reverse
		              : Direction::normal;
	}

	Duration getPosition() const { return position; };

	bool isRunning() const {
		return playState == PlayState::running;
	};

	bool isReversed() const {
		return direction == Direction::reverse;
	};

	bool atEndPosition() const;

protected:
	bool changed;
	Controllable &controlled;
	Duration position;
	Duration lastPosition;
	PlayState playState;
	Direction direction;
	TimePoint actTime;
	Event onFinish;
	Event onChange;

	void setOnFinish(Event onFinish);
	void setOnChange(Event onChange);
	void reset();
};

}

#endif
