#include "control.h"

#include <utility>

#include "base/text/valueunit.h"

Anim::Control::Control(Controllable &controlled) :
    controlled(controlled),
    position(Duration(0.0)),
    lastPosition(Duration(0.0))
{}

void Anim::Control::setOnFinish(OnFinish onFinish)
{
	this->onFinish = std::move(onFinish);
}

void Anim::Control::setOnChange(OnChange onChange)
{
	this->onChange = std::move(onChange);
}

void Anim::Control::seek(const std::string &value)
{
	if (const Text::ValueUnit vu(value); vu.getUnit() == "%")
		seekProgress(vu.getValue() / 100.0);
	else
		seekTime(Duration(value));
}

void Anim::Control::seekProgress(double value)
{
	seekTime(controlled.getDuration() * value);
}

double Anim::Control::getProgress() const
{
	auto duration = static_cast<double>(controlled.getDuration());
	return duration == 0 ? 0
	                     : static_cast<double>(position) / duration;
}

void Anim::Control::seekTime(Duration pos)
{
	position = pos;

	if (position > controlled.getDuration()) {
		playState = PlayState::paused;
		position = controlled.getDuration();
	}
	else if (position < Duration(0)) {
		playState = PlayState::paused;
		position = Duration(0);
	}

	update();
}

bool Anim::Control::atStartPosition() const
{
	return position == Duration(0.0);
}

bool Anim::Control::atEndPosition() const
{
	return position == controlled.getDuration();
}

bool Anim::Control::atIntermediatePosition() const
{
	return !atStartPosition() && !atEndPosition();
}

void Anim::Control::reset()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	actTime = TimePoint();
	cancelled = false;
	finished = false;
}

void Anim::Control::stop()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	update();
}

void Anim::Control::cancel()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	cancelled = true;
	update();
}

void Anim::Control::update() { update(actTime); }

void Anim::Control::update(const TimePoint &time)
{
	if (actTime == TimePoint()) actTime = time;

	const Duration step{time - std::exchange(actTime, time)};
	auto running = playState == PlayState::running;

	if (running && step != Duration(0.0)) {
		if (direction == Direction::normal)
			seekTime(position + step);
		else
			seekTime(position - step);
	}

	if (lastPosition != position) {
		controlled.setPosition(position);
		if (onChange) onChange();
	}

	lastPosition = position;

	finish(running);
}

void Anim::Control::finish(bool preRun)
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