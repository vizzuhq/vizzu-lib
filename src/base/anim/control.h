#ifndef ANIM_CONTROL
#define ANIM_CONTROL

#include <functional>

#include "base/anim/controllable.h"
#include "base/refl/enum.h"
#include "base/util/event.h"

namespace Anim
{

class Control
{
public:
	class Enum(PlayState)( paused, running );
	class Enum(Direction)( normal, reverse );

	typedef std::function<void()> OnChange;
	typedef std::function<void(bool)> OnFinish;

	Control(Controllable &controlled);
	void update(const TimePoint &time);

	void seek(const std::string &value);
	void seekProgress(double value);
	void seekTime(Duration pos);
	void pause() { playState = PlayState::paused; }
	void play() { playState = PlayState::running; }
	void setPlayState(PlayState state) { playState = state; }
	void setDirection(Direction dir) { direction = dir; }
	void stop();
	void cancel();

	void reverse() {
		direction = direction == Direction::normal
		              ? Direction::reverse
		              : Direction::normal;
	}

	Duration getPosition() const { return position; };
	double getProgress() const;

	bool isRunning() const {
		return playState == PlayState::running;
	};

	bool isReversed() const {
		return direction == Direction::reverse;
	};

	bool atStartPosition() const;
	bool atEndPosition() const;
	bool atIntermediatePosition() const;

	Util::Event<> onBegin;
	Util::Event<> onComplete;

protected:
	bool changed;
	bool cancelled;
	bool finished;
	Controllable &controlled;
	Duration position;
	Duration lastPosition;
	PlayState playState;
	Direction direction;
	TimePoint actTime;
	OnFinish onFinish;
	OnChange onChange;

	void setOnFinish(OnFinish onFinish);
	void setOnChange(OnChange onChange);
	void reset();
	void update();
};

}

#endif
