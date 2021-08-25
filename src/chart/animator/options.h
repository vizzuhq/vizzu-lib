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
	(style,title,legend,visible,color,coordSystem,geometry,y,x);

class Options
{
public:
	Options() : playState(::Anim::Control::PlayState::running) {}

	struct Section {
		std::optional<::Anim::Easing> easing;
		std::optional<::Anim::Duration> delay;
		std::optional<::Anim::Duration> duration;
		void set(const std::string &param, const std::string &value);
		void writeOver(::Anim::Options &option) const;
	};

	Section all;
	::Anim::Control::PlayState playState;
	std::array<Section, SectionId::EnumInfo::count()> sections;

	void set(const std::string &path, const std::string &value);

	::Anim::Options get(SectionId sectionId, 
		const ::Anim::Options &defaultOption) const;
};

}
}

#endif
