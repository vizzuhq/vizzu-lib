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

template <typename T, typename = void> class StyleMorph;

template <class T>
class StyleMorph<T,
    std::enable_if_t<
        Type::is_optional_v<T>
        && Math::Niebloid::interpolatable<typename T::value_type>>> :
    public ::Anim::IElement
{
public:
	StyleMorph(const T &source, const T &target, T &actual) :
	    source(source),
	    target(target),
	    actual(actual)
	{}

	void transform(double factor) override
	{
		*actual =
		    Math::Niebloid::interpolate(*source, *target, factor);
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
	    requires(requires(StyleMorph<T> &m) { m.transform(0.0); })
	void operator()(const T &source, const T &target, T &value) const;

	template <typename T>
	    requires(
	        std::is_same_v<typename T::value_type, Text::NumberFormat>
	        || std::is_same_v<typename T::value_type,
	            Text::NumberScale>
	        || std::is_same_v<typename T::value_type,
	            Styles::MarkerLabel::Format>)
	void operator()(const T &, const T &, T &) const;

private:
	mutable bool needed{};
	Styles::Chart *pActual;
	const Styles::Chart *pSource;
	const Styles::Chart *pTarget;
	::Anim::Group *group{};
	const ::Anim::Options *options{};
};

}

#endif
