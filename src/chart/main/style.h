#ifndef STYLE_H
#define STYLE_H

#include <cstdint>

#include "base/anim/interpolated.h"
#include "base/geom/angle.h"
#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/font.h"
#include "base/gfx/length.h"
#include "base/gui/accessories.h"
#include "base/style/param.h"
#include "base/text/numberscale.h"
#include "base/text/smartstring.h"
#include "chart/options/channel.h"

namespace Vizzu::Styles
{

using ::Style::Param;

enum class Visibility : std::uint8_t { hidden, visible };
enum class Overflow : std::uint8_t { hidden, visible };

struct Padding
{
	Param<Gfx::Length> paddingTop;
	Param<Gfx::Length> paddingRight;
	Param<Gfx::Length> paddingBottom;
	Param<Gfx::Length> paddingLeft;

	[[nodiscard]] GUI::Margin toMargin(const Geom::Size &size,
	    double fontSize) const
	{
		return {paddingTop->get(size.y, fontSize),
		    paddingLeft->get(size.x, fontSize),
		    paddingBottom->get(size.y, fontSize),
		    paddingRight->get(size.x, fontSize)};
	}

	[[nodiscard]] GUI::Margin toInvMargin(const Geom::Size &size,
	    double fontSize) const
	{
		return toMargin(extendSize(size, fontSize), fontSize);
	}

	[[nodiscard]] Geom::Rect contentRect(const Geom::Rect &rect,
	    double fontSize,
	    bool flip = false) const
	{
		auto margin = toMargin(rect.size, fontSize);
		return {
		    rect.pos
		        + (flip ? margin.bottomRight() : margin.topLeft()),
		    Geom::Size(rect.size - margin.getSpace()).positive()};
	}

	[[nodiscard]] Geom::Size extendSize(const Geom::Size &size,
	    double fontSize) const
	{
		return {size.x
		            + (*paddingLeft + *paddingRight)
		                  .fromNet(size.x, fontSize),
		    size.y
		        + (*paddingTop + *paddingBottom)
		              .fromNet(size.y, fontSize)};
	}
};

struct Font
{
	Param<::Anim::String> fontFamily;
	Param<Gfx::Font::Style> fontStyle;
	Param<Gfx::Font::Weight> fontWeight;
	Param<Gfx::Length> fontSize;
	const Font *fontParent = nullptr;

	[[nodiscard]] consteval static auto members()
	{
		return std::tuple{&Font::fontFamily,
		    &Font::fontStyle,
		    &Font::fontWeight,
		    &Font::fontSize,
		    std::ignore};
	}

	[[nodiscard]] double calculatedSize() const
	{
		if (fontSize.has_value() && fontSize->isAbsolute())
			return fontSize->get();

		if (fontSize.has_value() && fontParent)
			return fontSize->get(fontParent->calculatedSize(),
			    fontParent->calculatedSize());

		if (fontParent) return fontParent->calculatedSize();

		throw std::logic_error("internal error: no font parent set");
	}

	[[nodiscard]] Text::immutable_string calculatedFamily() const
	{
		if (fontFamily.has_value())
			if (auto &&ff =
			        fontFamily->get_or_first(::Anim::first).value;
			    !ff.empty())
				return ff;

		if (fontParent) return fontParent->calculatedFamily();

		throw std::logic_error("internal error: no font parent set");
	}

	explicit operator Gfx::Font() const
	{
		return {calculatedFamily(),
		    *fontStyle,
		    *fontWeight,
		    calculatedSize()};
	}
};

struct Text
{
	enum class TextAlign : std::int8_t {
		left = -1,
		center = 0,
		right = 1
	};

	Param<Gfx::Color> color;
	Param<::Anim::Interpolated<TextAlign>> textAlign;
	Param<Gfx::Color> backgroundColor;
	Param<::Text::NumberFormat> numberFormat;
	Param<double> maxFractionDigits;
	Param<::Text::NumberScale> numberScale;
};

struct Box
{
	Param<Gfx::Color> backgroundColor;
	Param<Gfx::Color> borderColor;
	Param<double> borderWidth;
};

struct Label : Padding, Font, Text
{
	[[nodiscard]] double getHeight() const
	{
		auto font = Gfx::Font{*this};
		auto textHeight = Gfx::ICanvas::textBoundary(font, "").y;
		return paddingTop->get(textHeight, font.size)
		     + paddingBottom->get(textHeight, font.size) + textHeight;
	}
};

struct Tick
{
	enum class Position : std::uint8_t { outside, inside, center };

	Param<Gfx::Color> color;
	Param<double> lineWidth;
	Param<Gfx::Length> length;
	Param<::Anim::Interpolated<Position>> position;
};

struct Guide
{
	Param<Gfx::Color> color;
	Param<double> lineWidth;
};

struct Interlacing
{
	Param<Gfx::Color> color;
};

struct OrientedLabelParams
{
	enum class Orientation : std::uint8_t {
		normal,
		tangential,
		horizontal,
		vertical
	};

	Param<::Anim::Interpolated<Orientation>> orientation;
	Param<Geom::Angle180> angle;
};

struct OrientedLabel : Label, OrientedLabelParams
{};

struct AxisLabelParams
{
	enum class Position : std::uint8_t { axis, min_edge, max_edge };

	enum class Side : std::uint8_t { positive, negative };

	Param<::Anim::Interpolated<Position>> position;
	Param<::Anim::Interpolated<Side>> side;
};

struct AxisLabel : OrientedLabel, AxisLabelParams
{};

consteval auto unique_enum_names(AxisLabelParams::Position)
{
	return "axis,min-edge,max-edge";
}

struct AxisTitleParams
{
	enum class Position : std::uint8_t { axis, min_edge, max_edge };

	enum class Side : std::uint8_t { positive, upon, negative };
	enum class VPosition : std::uint8_t { begin, middle, end };
	enum class VSide : std::uint8_t { positive, upon, negative };
	enum class Orientation : std::uint8_t { horizontal, vertical };

	Param<::Anim::Interpolated<Position>> position;
	Param<::Anim::Interpolated<Side>> side;
	Param<::Anim::Interpolated<VPosition>> vposition;
	Param<::Anim::Interpolated<VSide>> vside;
	Param<::Anim::Interpolated<Orientation>> orientation;
};

struct AxisTitle : Label, AxisTitleParams
{};

consteval auto unique_enum_names(AxisTitleParams::Position)
{
	return "axis,min-edge,max-edge";
}

struct Axis
{
	Param<Gfx::Color> color;
	AxisTitle title;
	AxisLabel label;
	Tick ticks;
	Guide guides;
	Interlacing interlacing;
};

struct MarkerLabelParams
{
	enum class Position : std::uint8_t {
		center,
		left,
		right,
		top,
		bottom
	};
	enum class Format : std::uint8_t {
		measureFirst,
		dimensionsFirst
	};

	Param<::Anim::Interpolated<Position>> position;
	Param<Gfx::ColorTransform> filter;
	Param<Format> format;
};

struct MarkerLabel : OrientedLabel, MarkerLabelParams
{};

struct TooltipParams
{
	enum class Layout : std::uint8_t { singleLine, multiLine };

	Param<::Anim::Interpolated<Layout>> layout;
	Param<Gfx::Color> color;
	Param<Gfx::Color> shadowColor;
	Param<double> borderRadius;
	Param<double> dropShadow;
	Param<double> arrowSize;
	Param<double> distance;
	Param<::Anim::String> seriesName;
};

struct Tooltip : Font, Box, TooltipParams
{};

struct DataPoint
{
	Param<Gfx::ColorGradient> colorGradient;
	Param<::Anim::Interpolated<Gfx::ColorPalette>> colorPalette;
	Param<double> minLightness;
	Param<double> maxLightness;
	Param<double> lineMinWidth;
	Param<double> lineMaxWidth;
	Param<double> circleMinRadius;
	Param<double> circleMaxRadius;
	Param<::Anim::Interpolated<std::optional<double>>>
	    rectangleSpacing;

	[[nodiscard]] Math::Range<double> lightnessRange() const
	{
		return Math::Range<double>::Raw(*minLightness, *maxLightness);
	}
};

struct MarkerParams
{
	enum class BorderOpacityMode : std::uint8_t {
		straight,
		premultiplied
	};

	Param<double> borderWidth;
	Param<double> borderOpacity;
	Param<::Anim::Interpolated<BorderOpacityMode>> borderOpacityMode;
	Param<double> fillOpacity;
	Guide guides;
	MarkerLabel label;
};

struct Marker : DataPoint, MarkerParams
{};

struct LegendParams
{
	struct Marker
	{
		enum class Type : std::uint8_t { circle, square };

		Param<::Anim::Interpolated<Type>> type;
		Param<Gfx::Length> size;
	};

	Param<Gfx::Length> width;
	Param<Gfx::Length> maxWidth;
	Label title;
	Label label;
	Marker marker;
	Param<Gfx::Length> translateY;
};

struct Legend : Padding, Box, LegendParams
{
	[[nodiscard]] double computedWidth(double refSize,
	    double fontSize) const
	{
		return std::min(width->get(refSize, fontSize),
		    maxWidth->get(refSize, fontSize),
		    Math::Floating::less);
	}
};

struct PlotParams
{
	Marker marker;
	Axis xAxis;
	Axis yAxis;
	Param<Gfx::Color> areaColor;
	Param<::Anim::Interpolated<Overflow>> overflow;
};

struct Plot : Padding, Box, PlotParams
{
	[[nodiscard]] const Axis &getAxis(Gen::ChannelId id) const
	{
		return id == Gen::ChannelId::x ? xAxis : yAxis;
	}
};

struct LogoParams
{
	Param<Gfx::Length> width;
	Param<Gfx::ColorTransform> filter;
};

struct Logo : Padding, LogoParams
{};

struct ChartParams
{
	Plot plot;
	Legend legend;
	Label title;
	Label subtitle;
	Label caption;
	Tooltip tooltip;
	Logo logo;
};

struct Chart : Padding, Box, Font, ChartParams
{
	static const Font &getDefaultFont();
	static const Gfx::ColorPalette &getDefaultColorPalette();
	static Chart def();

	void setup();
};

}

#endif
