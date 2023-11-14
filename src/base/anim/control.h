#ifndef BASE_ANIM_CONTROL_H
#define BASE_ANIM_CONTROL_H

#include <functional>

#include "base/anim/controllable.h"
#include "base/util/event.h"

namespace Anim
{

class Control
{
public:
	enum class PlayState { paused, running };
	enum class Direction { normal, reverse };

	using OnChange = std::function<void()>;
	using OnFinish = std::function<void(bool)>;

	Control(const Control &) = default;
	Control(Control &&) noexcept = default;

	explicit Control(Controllable &controlled);
	void update(const TimePoint &time);

	void seek(const std::string &value);
	void seekProgress(double value);
	void seekTime(Duration pos);
	void pause() { playState = PlayState::paused; }
	void play() { playState = PlayState::running; }
	void setPlayState(PlayState state) { playState = state; }
	void setDirection(Direction dir) { direction = dir; }
	void setSpeed(double speed);
	void stop();
	void cancel();

	void reverse()
	{
		direction = direction == Direction::normal
		              ? Direction::reverse
		              : Direction::normal;
	}

	[[nodiscard]] Duration getPosition() const;
	[[nodiscard]] double getProgress() const;

	[[nodiscard]] bool isRunning() const
	{
		return playState == PlayState::running;
	};

	[[nodiscard]] bool isReversed() const
	{
		return direction == Direction::reverse;
	};

	[[nodiscard]] bool atStartPosition() const;
	[[nodiscard]] bool atEndPosition() const;
	[[nodiscard]] bool atIntermediatePosition() const;

	Util::Event<> onBegin;
	Util::Event<> onComplete;

protected:
	bool changed{};
	bool cancelled{};
	bool finished{};
	Controllable &controlled;
	double progress{0.0};
	double lastProgress{0.0};
	PlayState playState{PlayState::paused};
	Direction direction{Direction::normal};
	double speed{1.0};
	TimePoint actTime;
	OnFinish onFinish;
	OnChange onChange;

	void setOnFinish(OnFinish onFinish);
	void setOnChange(OnChange onChange);
	void reset();
	void update();

private:
	void finish(bool preRun);
	void setProgress(double value);
	void setPosition(Duration pos);
	[[nodiscard]] double positionToProgress(Duration pos) const;
};

}

#endif
