#include "base/anim/group.h"

using namespace Anim;

void Group::setBaseline()
{
	baseline = duration;
}

void Group::resetBaseline()
{
	baseline = Duration(0.0);
}

void Group::addElement(
	std::unique_ptr<IElement> element,
    Options options)
{
	options.delay += baseline;
	elements.emplace_back(std::move(element), options);
	if (options.end() > duration) duration = options.end();
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
