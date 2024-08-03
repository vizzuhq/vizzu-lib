#ifndef CHART_ANIM_OPTIONS
#define CHART_ANIM_OPTIONS

#include <cstdint>
#include <optional>

#include "base/anim/control.h"
#include "base/anim/options.h"
#include "base/refl/auto_enum.h"

namespace Vizzu::Anim
{

enum class SectionId : std::uint8_t {
	style,
	title,
	subtitle,
	caption,
	legend,
	show,
	hide,
	color,
	coordSystem,
	geometry,
	y,
	x,
	tooltip,
	connection
};

enum class RegroupStrategy : std::uint8_t {
	fade,
	drilldown,
	aggregate
};

class Options
{
public:
	struct Section
	{
		std::optional<::Anim::Easing> easing;
		std::optional<::Anim::Duration> delay;
		std::optional<::Anim::Duration> duration;
		void set(const std::string &param, const std::string &value);
		[[nodiscard]] bool isSet() const;
	};

	struct Keyframe
	{
		std::optional<RegroupStrategy> regroupStrategy;
		Section all;
		Refl::EnumArray<SectionId, Section> sections;
		Section &get(SectionId sectionId);
		[[nodiscard]] const Section &get(SectionId sectionId) const;
		[[nodiscard]] RegroupStrategy getRegroupStrategy() const;
	};

	Keyframe keyframe;
	::Anim::Control::Option control;

	void set(const std::string &path, const std::string &value);
};

}

#endif
