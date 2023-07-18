#include "chart/animator/styles.h"

#include "base/refl/auto_struct.h"

using namespace Vizzu;
using namespace Vizzu::Anim::Morph;
using namespace Math;
StyleMorphFactory::StyleMorphFactory(const Styles::Chart &source,
    const Styles::Chart &target,
    Styles::Chart &actual)
	: dry{true}
	, needed{false}
    , pActual(std::addressof(actual))
	, pSource(std::addressof(source))
	, pTarget(std::addressof(target))
{}


static_assert(std::is_void_v<
	decltype(std::declval<StyleMorphFactory&>() (
		std::declval<const std::optional<Gfx::Length>&>(), 
		std::declval<const std::optional<Gfx::Length>&>(),
		std::declval<std::optional<Gfx::Length>&>()))
>);

bool StyleMorphFactory::isNeeded()
{
	dry = true;
	needed = false;
	Refl::visit(*this, *pSource, *pTarget, *pActual);
	return needed;
}

void StyleMorphFactory::populate(::Anim::Group &group,
    const ::Anim::Options &options)
{
	this->group = &group;
	this->options = &options;
	dry = false;
	Refl::visit(*this, *pSource, *pTarget, *pActual);
}

template <typename T>
auto StyleMorphFactory::operator()(
    const T &source, const T &target, T &value
) -> std::void_t<decltype(std::declval<StyleMorph<T>&>().transform(0.0))>
{
	if (*source != *target) {
		if (this->dry)
			this->needed = true;
		else {
			auto morph = std::make_unique<StyleMorph<T>>(source,
			    target,
			    value);

			if (group && options)
				group->addElement(std::move(morph), *options);
		}
	}
}

template <typename T>
auto StyleMorphFactory::operator()(const T&, const T &, T &)
    -> std::enable_if_t<
        std::is_same_v<typename T::value_type, Text::NumberFormat>
        || std::is_same_v<typename T::value_type, Text::NumberScale>
        || std::is_same_v<typename T::value_type,
            Styles::MarkerLabel::Format>
        || std::is_same_v<typename T::value_type,
            Gfx::ColorPalette>> {}
