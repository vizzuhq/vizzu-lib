#ifndef CHART_ANIM_OPTIONS
#define CHART_ANIM_OPTIONS

#include <array>
#include <optional>

#include "base/anim/options.h"
#include "base/anim/control.h"
#include "base/refl/enum.h"

namespace Vizzu
{
namespace Anim
{

class Enum(SectionId)
	(style,title,legend,show,hide,color,coordSystem,geometry,y,x,tooltip);

class Options
{
public:

	struct Section {
		std::optional<::Anim::Easing> easing;
		std::optional<::Anim::Duration> delay;
		std::optional<::Anim::Duration> duration;
		void set(const std::string &param, const std::string &value);
	};

	struct Keyframe {
		Section all;
		std::array<Section, SectionId::EnumInfo::count()> sections;
		const Section &get(SectionId sectionId) const;
	};

	struct Control {
		::Anim::Control::PlayState playState;
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
