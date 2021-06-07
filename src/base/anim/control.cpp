#include "control.h"

#include "base/text/valueunit.h"

using namespace Anim;

Control::Control(Controllable &controlled) :
    controlled(controlled),
    position(Duration(0.0)),
    playState(PlayState::paused),
    direction(Direction::normal)
{
	finished = false;
}

void Control::setOnFinish(Event onFinish)
{
	this->onFinish = std::move(onFinish);
}

void Control::setOnChange(Event onChange)
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
	seekTime(controlled.getDuration() *value);
}

void Control::seekTime(Duration pos)
{
	position = pos;

	if (position > controlled.getDuration())
	{
		playState = PlayState::paused;
		position = controlled.getDuration();
	}
	else if (position < Duration(0)) {
		playState = PlayState::paused;
		position = Duration(0);
	}
}

bool Control::atEndPosition() const
{
	return position == endPosition();
}

Duration Control::endPosition() const
{
	return direction == Direction::normal ? controlled.getDuration()
	                                      : Duration(0);
}

void Control::reset()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
	actTime = TimePoint();
}

void Control::stop()
{
	playState = PlayState::paused;
	direction = Direction::normal;
	position = Duration(0.0);
}

void Control::update(const TimePoint &time)
{
	if (finished)
	{
		onFinish();
		finished = false;
	}

	if (actTime == TimePoint()) actTime = time;

	Duration step = time - actTime;
	actTime = time;
	bool running = playState == PlayState::running;

	if (running)
	{
		if (direction == Direction::normal)
			seekTime(position + step);
		else
			seekTime(position - step);
	}

	if (lastPosition != position)
	{
		controlled.setPosition(position);
		if (onChange) onChange();
	}

	lastPosition = position;

	if (running && atEndPosition() && onFinish) finished = true;
}
