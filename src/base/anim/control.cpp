#include "control.h"

#include <utility>

#include "base/text/valueunit.h"

namespace Anim
{

Control::Control(Controllable &controlled) : controlled(controlled) {}

void Control::seek(const std::string &value)
{
	if (const Text::ValueUnit vu(value); vu.getUnit() == "%")
		seekProgress(vu.getValue() / 100.0);
	else
		seekTime(Duration(value));
}

Duration Control::getPosition() const
{
	return controlled.getDuration() * options.position;
}

double Control::getProgress() const { return options.position; }

void Control::seekProgress(double value) { setProgress(value); }

void Control::setProgress(double value)
{
	options.position = value;

	if (options.position > 1.0) {
		options.playState = PlayState::paused;
		options.position = 1.0;
	}
	else if (options.position < 0.0) {
		options.playState = PlayState::paused;
		options.position = 0.0;
	}
}

void Control::seekTime(Duration pos)
{
	seekProgress(positionToProgress(pos));
}

void Control::setPosition(Duration pos)
{
	setProgress(positionToProgress(pos));
}

double Control::positionToProgress(Duration pos) const
{
	return pos == Duration(0.0) ? 0.0
	                            : pos / controlled.getDuration();
}

void Control::setSpeed(double speed)
{
	options.speed = std::max(0.0, speed);
	update();
}

bool Control::atStartPosition() const
{
	return options.position <= 0.0;
}

bool Control::atEndPosition() const
{
	return options.position >= 1.0;
}

void Control::reset()
{
	options = {PlayState::paused};
	actTime = TimePoint();
	cancelled = false;
	finished = false;
}

void Control::stop()
{
	options.playState = PlayState::paused;
	options.direction = Direction::normal;
	options.position = 0.0;
}

void Control::cancel()
{
	options.playState = PlayState::paused;
	options.direction = Direction::normal;
	options.position = 0.0;
	cancelled = true;
}

void Control::update() { update(actTime); }

Control::PlayState Control::update(const TimePoint &time)
{
	if (actTime == TimePoint()) actTime = time;

	auto running = options.playState == PlayState::running;

	Duration timeStep{time - std::exchange(actTime, time)};

	timeStep = timeStep * options.speed
	         * (options.direction == Direction::normal ? 1.0 : -1.0);

	if (running && timeStep != Duration(0.0)) {
		setPosition(getPosition() + timeStep);
	}

	if (lastPosition != options.position) {
		controlled.setPosition(getPosition());
		onChange();
	}

	lastPosition = options.position;

	finish(running);

	return options.playState;
}

void Control::finish(bool preRun)
{
	if (cancelled) {
		cancelled = false;
		if (!finished) {
			onFinish(false);
			finished = true;
		}
	}
	else if (preRun
	         && ((options.direction == Direction::normal
	                 && atEndPosition())
	             || (options.direction == Direction::reverse
	                 && atStartPosition()))
	         && options.playState != PlayState::running) {
		if (!finished) {
			onFinish(true);
			finished = true;
		}
	}
}

}