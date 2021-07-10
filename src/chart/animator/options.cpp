#include "options.h"

#include "base/conv/parse.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

void Options::set(const std::string &path,
	const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');

	if (parts.size() != 2) 
		throw std::logic_error("invalid animation option: " + path);

	auto sectionId = SectionId(parts[0]);
	auto &section = sections.at((int)sectionId);

	auto option = parts[1];
	if (option == "easing")
		section.easing = ::Anim::Easing(value);
	else if (option == "delay")
		section.delay = ::Anim::Duration(value);
	else if (option == "duration")
		section.duration = ::Anim::Duration(value);
	else throw std::logic_error("invalid animation option: " + option);
}

::Anim::Options Options::get(SectionId sectionId, 
	const ::Anim::Options &defaultOption) const
{
	auto &section = sections.at((int)sectionId);
	auto res = defaultOption;
	if (section.easing.has_value()) res.easing = *section.easing;
	if (section.delay.has_value()) res.delay = *section.delay;
	if (section.duration.has_value()) res.duration = *section.duration;
	return res;
}
