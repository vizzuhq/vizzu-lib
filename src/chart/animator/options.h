#ifndef CHART_ANIM_OPTIONS
#define CHART_ANIM_OPTIONS

#include <array>
#include <optional>

#include "base/anim/options.h"
#include "base/refl/enum.h"

namespace Vizzu
{
namespace Anim
{

class Enum(SectionId)
	(style,title,enable,color,coordsys,shape,vertical,horizontal);

class Options
{
public:
	struct Section {
		std::optional<::Anim::Easing> easing;
		std::optional<::Anim::Duration> delay;
		std::optional<::Anim::Duration> duration;
	};

	std::array<Section, SectionId::EnumInfo::count()> sections;

	void set(const std::string &path, const std::string &value);
};

}
}

#endif
