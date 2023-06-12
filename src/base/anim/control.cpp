#include "control.h"

#include "base/text/valueunit.h"

using namespace Anim;

Control::Control(Controllable &controlled) :
    changed(false),
    cancelled(false),
    finished(false),
    controlled(controlled),
    position(Duration(0.0)),
    lastPosition(Duration(0.0)),
    playState(PlayState::paused),
    direction(Direction::normal),
    actTime(TimePoint())
{}

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
	Text::ValueUnit vu(value);

	if (vu.getUnit() == "%")
		seekProgress(vu.getValue() / 100.0);
	else
		seekTime(Duration(value));
}

void Control::seekProgress(double value)
{
	seekTime(controlled.getDuration() * value);
}

double Control::getProgress() const
{
	auto duration = (double)controlled.getDuration();
	return duration == 0 ? 0 : (double)position / duration;
}

void Control::seekTime(Duration pos)
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

bool Control::atStartPosition() const
{
	return position == Duration(0.0);
}

bool Control::atEndPosition() const
{
	return position == controlled.getDuration();
}

bool Control::atIntermediatePosition() const
{
	return !atStartPosition() && !atEndPosition();
}

void Control::reset()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	actTime = TimePoint();
	cancelled = false;
	finished = false;
}

void Control::stop()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	update();
}

void Control::cancel()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	cancelled = true;
	update();
}

void Control::update() { update(actTime); }

void Control::update(const TimePoint &time)
{
	if (actTime == TimePoint()) actTime = time;

	Duration step = time - actTime;
	actTime = time;
	bool running = playState == PlayState::running;

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

	if (cancelled) {
		cancelled = false;
		if (!finished && onFinish) {
			onFinish(false);
			finished = true;
		}
	}
	else if (running
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
