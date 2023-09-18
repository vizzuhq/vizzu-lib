#include "base/anim/group.h"

void Anim::Group::calcDuration()
{
	setDuration(::Anim::Duration());
	for (const auto &element : elements)
		if (element.options.end() > getDuration())
			setDuration(element.options.end());
}

void Anim::Group::reTime(Duration duration, Duration delay)
{
	if (duration == Duration(0)) duration = Duration(1);
	if (static_cast<double>(getDuration()) == 0.0) return;
	for (auto &element : elements)
		reTime(element.options, duration, delay);
	baseline = delay + baseline * (duration / getDuration());
	calcDuration();
}

void Anim::Group::reTime(Options &options,
    Duration duration,
    Duration delay)
{
	auto start = options.start();
	auto end = options.end();
	auto newStart = delay + start * (duration / getDuration());
	auto newEnd = delay + end * (duration / getDuration());
	options.delay = newStart;
	options.duration = newEnd - newStart;
}

void Anim::Group::setBaseline() { baseline = getDuration(); }

void Anim::Group::resetBaseline() { baseline = Duration(0.0); }

Anim::Duration Anim::Group::getBaseline() const { return baseline; }

const Anim::Options &Anim::Group::addElement(
    std::unique_ptr<IElement> element,
    Options options)
{
	options.delay += baseline;
	elements.emplace_back(std::move(element), options);
	if (options.end() > getDuration()) setDuration(options.end());
	return elements.back().options;
}

void Anim::Group::setPosition(Duration progress)
{
	for (const auto &element : elements) {
		auto factor = element.options.getFactor(progress);
		element.element->transform(factor);
	}
}

void Anim::Group::clear()
{
	elements.clear();
	baseline = Duration(0.0);
	setDuration(Duration(0.0));
}
