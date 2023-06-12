#ifndef CHART_ANIM_OPTIONS
#define CHART_ANIM_OPTIONS

#include <array>
#include <optional>

#include "base/anim/control.h"
#include "base/anim/options.h"
#include "base/refl/enum.h"

namespace Vizzu
{
namespace Anim
{

class Enum(SectionId)
    (style,title,legend,show,hide,color,coordSystem,geometry,y,x,tooltip);

class Enum(RegroupStrategy)(fade,drilldown,aggregate);

class Options
{
public:
	struct Section
	{
		std::optional<::Anim::Easing> easing;
		std::optional<::Anim::Duration> delay;
		std::optional<::Anim::Duration> duration;
		void set(const std::string &param, const std::string &value);
		bool isSet() const;
	};

	struct Keyframe
	{
		std::optional<RegroupStrategy> regroupStrategy;
		Section all;
		std::array<Section, SectionId::EnumInfo::count()> sections;
		Section &get(SectionId sectionId);
		const Section &get(SectionId sectionId) const;
		RegroupStrategy getRegroupStrategy() const;
	};

	struct Control
	{
		::Anim::Control::PlayState playState;
		::Anim::Control::Direction direction;
		double position;
		Control();
	};

	Keyframe keyframe;
	Control control;

	void set(const std::string &path, const std::string &value);
};

}
}

#endif
