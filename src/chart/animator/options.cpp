#include "options.h"

#include "base/conv/parse.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

using namespace std::literals::chrono_literals;

Options::Options()
{
	playState = ::Anim::Control::PlayState::running;
	position = 0.0;
}

void Options::Section::set(const std::string &param, const std::string &value)
{
	if (param == "easing") easing = ::Anim::Easing(value);
	else if (param == "delay") delay = ::Anim::Duration(value);
	else if (param == "duration") duration = ::Anim::Duration(value);
	else throw std::logic_error("invalid animation parameter: " + param);
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
		else if (path == "position") {
			position = Conv::parse<double>(value);
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

const Options::Section &Options::get(SectionId sectionId) const
{
	return sections.at((int)sectionId);
}
