#include "options.h"

#include <stdexcept>
#include <string>

#include "base/anim/control.h"
#include "base/anim/duration.h"
#include "base/anim/easing.h"
#include "base/conv/parse.h"
#include "base/text/smartstring.h"

namespace Vizzu::Anim
{

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
			control.playState =
			    Conv::parse<::Anim::Control::PlayState>(value);
		}
		else if (path == "position") {
			control.position = Conv::parse<double>(value);
		}
		else if (path == "direction") {
			control.direction =
			    Conv::parse<::Anim::Control::Direction>(value);
		}
		else if (path == "speed") {
			control.speed = Conv::parse<double>(value);
		}
		else if (path == "regroupStrategy") {
			keyframe.regroupStrategy =
			    Conv::parse<RegroupStrategy>(value);
		}
		else
			keyframe.all.set(path, value);
	}
	else if (parts.size() == 2) {
		auto sectionId = Conv::parse<SectionId>(parts[0]);
		keyframe.sections[sectionId].set(parts[1], value);
	}
	else
		throw std::logic_error("invalid animation option: " + path);
}

Options::Section &Options::Keyframe::get(SectionId sectionId)
{
	return sections[sectionId];
}

const Options::Section &Options::Keyframe::get(
    SectionId sectionId) const
{
	return sections[sectionId];
}

RegroupStrategy Options::Keyframe::getRegroupStrategy() const
{
	return regroupStrategy.value_or(RegroupStrategy::aggregate);
}

}