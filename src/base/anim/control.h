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

	struct Option
	{
		PlayState playState{PlayState::running};
		Direction direction{Direction::normal};
		double position{0.0};
		double speed{1.0};
	};

	Control(Control &&) noexcept = default;

	explicit Control(Controllable &controlled);
	void update(const TimePoint &time);

	void seek(const std::string &value);
	void seekProgress(double value);
	void seekTime(Duration pos);
	void stop();
	void cancel();

	Option &getOptions() { return options; }

	[[nodiscard]] Duration getPosition() const;
	[[nodiscard]] double getProgress() const;

	[[nodiscard]] bool isRunning() const
	{
		return options.playState == PlayState::running;
	};

	[[nodiscard]] bool atStartPosition() const;
	[[nodiscard]] bool atEndPosition() const;

	Util::Event<> onBegin;
	Util::Event<> onComplete;

	void update();

protected:
	bool cancelled{};
	bool finished{};
	Controllable &controlled;
	Option options{PlayState::paused};
	double lastPosition{0.0};

	TimePoint actTime;
	Util::Event<const bool> onFinish;
	Util::Event<> onChange;

	void reset();

private:
	void finish(bool preRun);
	void setProgress(double value);
	void setPosition(Duration pos);
	[[nodiscard]] double positionToProgress(Duration pos) const;
};

}

#endif
