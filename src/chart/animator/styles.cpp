#include "chart/animator/styles.h"

#include "base/math/interpolation.h"
#include "base/refl/auto_struct.h"

using namespace Vizzu;
using namespace Vizzu::Anim::Morph;
using namespace Math;

template <typename T> class StyleMorph : public ::Anim::IElement
{
public:
	StyleMorph(const T &source, const T &target, T &actual) :
	    source(source),
	    target(target),
	    actual(actual)
	{}

	void transform(double factor) override
	{
		*actual = interpolate(*source, *target, factor);
	}

	const T &source;
	const T &target;
	T &actual;
};

template <>
class StyleMorph<Style::Param<Gfx::Font::Style>> :
    public ::Anim::IElement
{
public:
	StyleMorph(const Style::Param<Gfx::Font::Style> &source,
	    const Style::Param<Gfx::Font::Style> &target,
	    Style::Param<Gfx::Font::Style> &actual) :
	    source(source),
	    target(target),
	    actual(actual)
	{}

	void transform(double factor) override
	{
		*actual = factor < 0.5 ? *source : *target;
	}

	const Style::Param<Gfx::Font::Style> &source;
	const Style::Param<Gfx::Font::Style> &target;
	Style::Param<Gfx::Font::Style> &actual;
};

StyleMorphFactory::StyleMorphFactory(const Styles::Chart &source,
    const Styles::Chart &target,
    Styles::Chart &actual)
{
	dry = true;
	needed = false;
	pSource = &source;
	pTarget = &target;
	pActual = &actual;
}

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
    const T &source, const T &target, const T &value
) -> std::void_t<decltype(interpolate(source, target, value))>
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
auto StyleMorphFactory::operator()(const T&, const T &, const T &)
    -> std::enable_if_t<
        std::is_same_v<typename T::value_type, Text::NumberFormat>
        || std::is_same_v<typename T::value_type, Text::NumberScale>
        || std::is_same_v<typename T::value_type,
            Styles::MarkerLabel::Format>
        || std::is_same_v<typename T::value_type,
            Gfx::ColorPalette>> {}
