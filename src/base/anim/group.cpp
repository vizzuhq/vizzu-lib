#include "base/anim/group.h"

#include "base/io/log.h"

using namespace Anim;

void Group::calcDuration()
{
	duration = ::Anim::Duration();
	for (const auto &element: elements)
		if (element.options.end() > duration) 
			duration = element.options.end();
}

void Group::reTime(Duration duration, Duration delay)
{
	if ((double)this->duration == 0.0) return;
	for (auto &element: elements) reTime(element.options, duration, delay);
	baseline = delay + baseline * (duration / this->duration);
	calcDuration();
}

void Group::reTime(Options &options, Duration duration, Duration delay)
{
	auto start = options.start();
	auto end = options.end();
	auto newStart = delay + start * (duration / this->duration);
	auto newEnd = delay + end * (duration / this->duration);
	options.delay = newStart;
	options.duration = newEnd - newStart;
}

void Group::setBaseline()
{
	baseline = duration;
}

void Group::resetBaseline()
{
	baseline = Duration(0.0);
}

Duration Group::getBaseline() const
{
	return baseline;
}

const Options &Group::addElement(
	std::unique_ptr<IElement> element,
    Options options)
{
	options.delay += baseline;
	elements.emplace_back(std::move(element), options);
	if (options.end() > duration) duration = options.end();
	return elements.back().options;
}

void Group::setPosition(Duration progress)
{
	for (const auto &element : elements)
	{
		auto factor = element.options.getFactor(progress);
		element.element->transform(factor);
	}
}

void Group::clear()
{
	elements.clear();
	baseline = Duration(0.0);
	duration = Duration(0.0);
}
