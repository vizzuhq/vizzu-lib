#ifndef CHART_ANIM_STYLES_H
#define CHART_ANIM_STYLES_H

#include <cstddef>

#include "base/anim/element.h"
#include "base/anim/group.h"
#include "base/math/interpolation.h"
#include "chart/main/style.h"

namespace Vizzu
{
namespace Anim
{
namespace Morph
{

class StyleMorphFactory
{
public:
	StyleMorphFactory(const Styles::Chart &source,
	    const Styles::Chart &target,
	    Styles::Chart &actual,
		::Anim::Group &group,
		const ::Anim::Options &options);

	template <typename T>
	StyleMorphFactory &operator()(T &value, const char *);

private:
	std::byte *pActual;
	const std::byte *pSource;
	const std::byte *pTarget;
	::Anim::Group &group;
	const ::Anim::Options &options;
};

}
}
}

#endif
