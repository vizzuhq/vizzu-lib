#ifndef CHART_ANIM_STYLES_H
#define CHART_ANIM_STYLES_H

#include <cstddef>
#include <type_traits>

#include "base/anim/element.h"
#include "base/anim/group.h"
#include "base/math/interpolation.h"
#include "chart/main/style.h"

namespace Vizzu::Anim::Morph
{

namespace
{
using namespace Math;
template <typename T, typename = void> class StyleMorph;

template <class T>
class StyleMorph<T,
    std::void_t<std::enable_if_t<Type::isoptional<T>::value>,
        decltype(interpolate(*std::declval<const T &>(),
            *std::declval<const T &>(),
            double{}))>> : public ::Anim::IElement
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
private:
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
private:
	const Style::Param<Gfx::Font::Style> &source;
	const Style::Param<Gfx::Font::Style> &target;
	Style::Param<Gfx::Font::Style> &actual;
};
}

class StyleMorphFactory
{
	void visit() const;

public:
	StyleMorphFactory(const Styles::Chart &source,
	    const Styles::Chart &target,
	    Styles::Chart &actual);

	bool isNeeded() const;
	void populate(::Anim::Group &group,
	    const ::Anim::Options &options);

	template <typename T>
	auto operator()(const T &source, const T &target, T &value) const
	    -> std::void_t<
	        decltype(std::declval<StyleMorph<T> &>().transform(0.0))>;

	template <typename T>
	auto
	operator()(const T &, const T &, T &) const -> std::enable_if_t<
	    std::is_same_v<typename T::value_type, Text::NumberFormat>
	    || std::is_same_v<typename T::value_type, Text::NumberScale>
	    || std::is_same_v<typename T::value_type,
	        Styles::MarkerLabel::Format>
	    || std::is_same_v<typename T::value_type, Gfx::ColorPalette>>;

private:
	mutable bool needed{};
	Styles::Chart *pActual;
	const Styles::Chart *pSource;
	const Styles::Chart *pTarget;
	::Anim::Group *group{};
	const ::Anim::Options *options;
};

}

#endif
