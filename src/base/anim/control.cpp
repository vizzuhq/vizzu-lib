#include "control.h"

#include <utility>

#include "base/text/valueunit.h"

namespace Anim
{

Control::Control(Controllable &controlled) : controlled(controlled) {}

void Control::setOnFinish(OnFinish onFinish)
{
	this->onFinish = std::move(onFinish);
}

void Control::setOnChange(OnChange onChange)
{
	this->onChange = std::move(onChange);
}

void Control::seek(const std::string &value)
{
	if (const Text::ValueUnit vu(value); vu.getUnit() == "%")
		seekProgress(vu.getValue() / 100.0);
	else
		seekTime(Duration(value));
}

Duration Control::getPosition() const
{
	return controlled.getDuration() * progress;
}

double Control::getProgress() const { return progress; }

void Control::seekProgress(double value)
{
	setProgress(value);
	update();
}

void Control::setProgress(double value)
{
	progress = value;

	if (progress > 1.0) {
		playState = PlayState::paused;
		progress = 1.0;
	}
	else if (progress < 0.0) {
		playState = PlayState::paused;
		progress = 0.0;
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
	this->speed = std::max(0.0, speed);
	update();
}

bool Control::atStartPosition() const { return progress <= 0.0; }

bool Control::atEndPosition() const { return progress >= 1.0; }

bool Control::atIntermediatePosition() const
{
	return !atStartPosition() && !atEndPosition();
}

void Control::reset()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	progress = 0.0;
	speed = 1.0;
	actTime = TimePoint();
	cancelled = false;
	finished = false;
}

void Control::stop()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	progress = 0.0;
	update();
}

void Control::cancel()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	progress = 0.0;
	cancelled = true;
	update();
}

void Control::update() { update(actTime); }

void Control::update(const TimePoint &time)
{
	if (actTime == TimePoint()) actTime = time;

	auto running = playState == PlayState::running;

	Duration timeStep{time - std::exchange(actTime, time)};

	timeStep = timeStep * speed
	         * (direction == Direction::normal ? 1.0 : -1.0);

	if (running && timeStep != Duration(0.0)) {
		setPosition(getPosition() + timeStep);
	}

	if (lastProgress != progress) {
		controlled.setPosition(getPosition());
		if (onChange) onChange();
	}

	lastProgress = progress;

	finish(running);
}

void Control::finish(bool preRun)
{
	if (cancelled) {
		cancelled = false;
		if (!finished && onFinish) {
			onFinish(false);
			finished = true;
		}
	}
	else if (preRun
	         && ((direction == Direction::normal && atEndPosition())
	             || (direction == Direction::reverse
	                 && atStartPosition()))
	         && playState != PlayState::running) {
		if (!finished && onFinish) {
			onFinish(true);
			finished = true;
		}
	}
}

}