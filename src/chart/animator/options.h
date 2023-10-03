#ifndef CHART_ANIM_OPTIONS
#define CHART_ANIM_OPTIONS

#include <array>
#include <optional>

#include "base/anim/control.h"
#include "base/anim/options.h"
#include "base/refl/auto_enum.h"

namespace Vizzu::Anim
{

enum class SectionId {
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

enum class RegroupStrategy { fade, drilldown, aggregate };

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

	struct Control
	{
		::Anim::Control::PlayState playState{
		    ::Anim::Control::PlayState::running};
		::Anim::Control::Direction direction{
		    ::Anim::Control::Direction::normal};
		double position{0.0};
		double speed{1.0};
	};

	Keyframe keyframe;
	Control control;

	void set(const std::string &path, const std::string &value);
};

}

#endif
