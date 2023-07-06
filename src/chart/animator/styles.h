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
	    Styles::Chart &actual);

	bool isNeeded();
	void populate(::Anim::Group &group,
	    const ::Anim::Options &options);

	template <typename T>
	auto operator()(const T &source, const T &target, const T &actual)
	    -> std::void_t<decltype(interpolate(source, target, actual))>;

	template <typename T>
	auto operator()(const T&, const T &, const T &)
	    -> std::enable_if_t<
	        std::is_same_v<typename T::value_type, Text::NumberFormat>
	        || std::is_same_v<typename T::value_type, Text::NumberScale>
	        || std::is_same_v<typename T::value_type,
	            Styles::MarkerLabel::Format>
	        || std::is_same_v<typename T::value_type,
	            Gfx::ColorPalette>>;

private:
	bool dry;
	bool needed;
	Styles::Chart *pActual;
	const Styles::Chart *pSource;
	const Styles::Chart *pTarget;
	::Anim::Group *group;
	const ::Anim::Options *options;
};

}
}
}

#endif
