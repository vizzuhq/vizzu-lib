#include "options.h"

#include "base/conv/parse.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::Anim;

using namespace std::literals::chrono_literals;

Options::Control::Control()
{
	playState = ::Anim::Control::PlayState::running;
	direction = ::Anim::Control::Direction::normal;
	position = 0.0;
}

void Options::Section::set(const std::string &param,
    const std::string &value)
{
	if (param == "easing")
		easing = ::Anim::Easing(value);
	else if (param == "delay")
		delay = ::Anim::Duration(value);
	else if (param == "duration")
		duration = ::Anim::Duration(value);
	else
		throw std::logic_error(
		    "invalid animation parameter: " + param);
}

bool Options::Section::isSet() const
{
	return easing || delay || duration;
}

void Options::set(const std::string &path, const std::string &value)
{
	auto parts = Text::SmartString::split(path, '.');

	if (parts.size() == 1) {
		if (path == "playState") {
			control.playState = ::Anim::Control::PlayState(value);
		}
		else if (path == "position") {
			control.position = Conv::parse<double>(value);
		}
		else if (path == "direction") {
			control.direction = ::Anim::Control::Direction(value);
		}
		else if (path == "regroupStrategy") {
			keyframe.regroupStrategy =
			    Conv::parse<RegroupStrategy>(value);
		}
		else
			keyframe.all.set(path, value);
	}
	else if (parts.size() == 2) {
		auto sectionId = SectionId(parts[0]);
		keyframe.sections.at((int)sectionId).set(parts[1], value);
	}
	else
		throw std::logic_error("invalid animation option: " + path);
}

Options::Section &Options::Keyframe::get(SectionId sectionId)
{
	return sections.at((int)sectionId);
}

const Options::Section &Options::Keyframe::get(
    SectionId sectionId) const
{
	return sections.at((int)sectionId);
}

RegroupStrategy Options::Keyframe::getRegroupStrategy() const
{
	return regroupStrategy
	         ? *regroupStrategy
	         : RegroupStrategy(RegroupStrategy::aggregate);
}
