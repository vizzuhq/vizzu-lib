#include "options.h"

#include "base/conv/parse.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

using namespace std::literals::chrono_literals;

Options::Options()
{
	playState = ::Anim::Control::PlayState::running;

	all.delay = ::Anim::Duration(0);
	all.duration = ::Anim::Duration(1500ms);
}

void Options::Section::set(const std::string &param, const std::string &value)
{
	if (param == "easing") easing = ::Anim::Easing(value);
	else if (param == "delay") delay = ::Anim::Duration(value);
	else if (param == "duration") duration = ::Anim::Duration(value);
	else throw std::logic_error("invalid animation parameter: " + param);
}

void Options::Section::writeOver(::Anim::Options &option) const
{
	if (easing.has_value()) option.easing = *easing;
	if (delay.has_value()) option.delay = *delay;
	if (duration.has_value()) option.duration = *duration;
}

void Options::set(const std::string &path,
	const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');

	if (parts.size() == 1) 
	{
		if (path == "playState") {
			playState = ::Anim::Control::PlayState(value);
		}
		else all.set(path, value);
	}
	else if (parts.size() == 2)
	{
		auto sectionId = SectionId(parts[0]);
		sections.at((int)sectionId).set(parts[1], value);
	}
	else throw std::logic_error("invalid animation option: " + path);
}

::Anim::Options Options::get(SectionId sectionId, 
	const ::Anim::Options &defaultOption) const
{
	auto &section = sections.at((int)sectionId);
	auto res = defaultOption;
	section.writeOver(res);
	return res;
}
