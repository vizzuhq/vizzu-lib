#include "chart/animator/styles.h"

#include "base/math/interpolation.h"
#include "base/refl/struct.h"

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
	pSource = reinterpret_cast<const std::byte *>(&source);
	pTarget = reinterpret_cast<const std::byte *>(&target);
	pActual = reinterpret_cast<std::byte *>(&actual);
}

bool StyleMorphFactory::isNeeded()
{
	dry = true;
	needed = false;
	reinterpret_cast<Styles::Chart *>(pActual)->visit(*this);
	return needed;
}

void StyleMorphFactory::populate(::Anim::Group &group,
    const ::Anim::Options &options)
{
	this->group = &group;
	this->options = &options;
	dry = false;
	reinterpret_cast<Styles::Chart *>(pActual)->visit(*this);
}

template <typename T>
StyleMorphFactory &StyleMorphFactory::operator()(T &value,
    const char *)
{
	if constexpr (Refl::isReflectable<T, StyleMorphFactory>) {
		value.visit(*this);
	}
	else if constexpr (
	    // todo: interpolate the following styles also
	    !std::is_same_v<typename T::value_type, Text::NumberFormat>
	    && !std::is_same_v<typename T::value_type, Text::NumberScale>
	    && !std::is_same_v<typename T::value_type,
	        Styles::MarkerLabel::Format>
	    && !std::is_same_v<typename T::value_type,
	        Gfx::ColorPalette>) {
		auto offset = reinterpret_cast<std::byte *>(&value) - pActual;
		const T &source =
		    *reinterpret_cast<const T *>(pSource + offset);
		const T &target =
		    *reinterpret_cast<const T *>(pTarget + offset);

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
	return *this;
}
