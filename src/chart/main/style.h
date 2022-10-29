#ifndef STYLE_H
#define STYLE_H

#include "base/anim/interpolated.h"
#include "base/math/fuzzybool.h"
#include "base/geom/angle.h"
#include "base/geom/rect.h"
#include "base/gfx/color.h"
#include "base/gfx/length.h"
#include "base/gfx/font.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colorpalette.h"
#include "base/gfx/colortransform.h"
#include "base/gui/accessories.h"
#include "base/refl/enum.h"
#include "base/style/param.h"
#include "base/text/smartstring.h"
#include "chart/options/scale.h"
#include "chart/generator/colorbuilder.h"

namespace Vizzu
{
namespace Styles
{

template <typename T> using Param = ::Style::Param<T>;

class Enum(Visibility)(hidden, visible);
class Enum(Overflow)(hidden, visible);

struct Padding
{
	Param<Gfx::Length> paddingTop;
	Param<Gfx::Length> paddingRight;
	Param<Gfx::Length> paddingBottom;
	Param<Gfx::Length> paddingLeft;

	GUI::Margin toMargin(const Geom::Size &size, double fontSize) const {
		return { 
			paddingTop->get(size.y, fontSize),
			paddingLeft->get(size.x, fontSize),
			paddingBottom->get(size.y, fontSize),
			paddingRight->get(size.x, fontSize) 
		};
	}

	Geom::Rect contentRect(const Geom::Rect &rect, double fontSize) const {
		auto margin = toMargin(rect.size, fontSize);
		return Geom::Rect(
			rect.pos + margin.topLeft(),
			Geom::Size(rect.size - margin.getSpace()).positive());
	}

	void visit(auto &visitor)
	{
		visitor
			(paddingTop, "paddingTop")
			(paddingBottom, "paddingBottom")
			(paddingLeft, "paddingLeft")
			(paddingRight, "paddingRight");
	}
};

struct Font {
	Param<::Anim::String> fontFamily;
	Param<Gfx::Font::Style> fontStyle;
	Param<Gfx::Font::Weight> fontWeight;
	Param<Gfx::Length> fontSize;
	const Font *fontParent = nullptr;

	double calculatedSize() const 
	{
		if (fontSize.has_value() && fontSize->isAbsolute()) 
			return fontSize->get();
		
		if (fontSize.has_value() && fontParent) 
			return fontSize->get(
				fontParent->calculatedSize(),
				fontParent->calculatedSize());
		
		if (fontParent)
			return fontParent->calculatedSize();

		throw std::logic_error("internal error: no font parent set");
	}

	std::string calculatedFamily() const 
	{
		if (fontFamily.has_value() && !fontFamily->values[0].value.empty())
			return fontFamily->values[0].value;
		
		if (fontParent) 
			return fontParent->calculatedFamily();
		
		throw std::logic_error("internal error: no font parent set");
	}

	explicit operator Gfx::Font() const
	{
		return Gfx::Font(calculatedFamily(),
			*fontStyle, *fontWeight, calculatedSize());
	}

	void visit(auto &visitor)
	{
		visitor
			(fontFamily, "fontFamily")
			(fontStyle, "fontStyle")
			(fontWeight, "fontWeight")
			(fontSize, "fontSize");
	}
};

struct Text {
	class Enum(TextAlign)(center, left, right);

	Param<Gfx::Color> color;
	Param<Anim::Interpolated<TextAlign>> textAlign;
	Param<Gfx::Color> backgroundColor;
	Param<::Text::NumberFormat> numberFormat;
	Param<double> maxFractionDigits;

	void visit(auto &visitor)
	{
		visitor
			(color, "color")
			(textAlign, "textAlign")
			(backgroundColor, "backgroundColor")
			(numberFormat, "numberFormat")
			(maxFractionDigits, "maxFractionDigits");
	}
};

struct Box {
	Param<Gfx::Color> backgroundColor;
	Param<Gfx::Color> borderColor;
	Param<double> borderWidth;

	void visit(auto &visitor)
	{
		visitor
		    (backgroundColor, "backgroundColor")
		    (borderColor, "borderColor")
		    (borderWidth, "borderWidth");
	}
};

struct Label : Padding, Font, Text
{
	void visit(auto &visitor)
	{
		Padding::visit(visitor);
		Font::visit(visitor);
		Text::visit(visitor);
	}
};

struct Tick {
	//todo> top, bottom, both
	class Enum(Position)(outside, inside, center);

	Param<Gfx::Color> color;
	Param<double> lineWidth;
	Param<Gfx::Length> length;
	Param<::Anim::Interpolated<Position>> position;

	void visit(auto &visitor)
	{
		visitor
			(color, "color")
			(lineWidth, "lineWidth")
			(length, "length")
			(position, "position");
	}
};

struct Guide
{
	Param<Gfx::Color> color;
	Param<double> lineWidth;

	void visit(auto &visitor)
	{
		visitor
			(color, "color")
			(lineWidth, "lineWidth");
	}
};

struct Interlacing
{
	Param<Gfx::Color> color;

	void visit(auto &visitor)
	{
		visitor
			(color, "color");
	}
};

struct OrientedLabel : Label
{
	class Enum(Orientation)(normal, tangential, horizontal, vertical);

	Param<::Anim::Interpolated<Orientation>> orientation;
	Param<Geom::Angle180> angle;

	void visit(auto &visitor)
	{
		Label::visit(visitor);
		visitor
			(orientation, "orientation")
			(angle, "angle");
	}
};

struct AxisLabel : OrientedLabel
{
	class SpecNameEnum(Position)
		(axis, min_edge, max_edge)
		(axis, min-edge, max-edge);
	
	class Enum(Side)(positive, negative);

	Param<::Anim::Interpolated<Position>> position;
	Param<::Anim::Interpolated<Side>> side;

	void visit(auto &visitor)
	{
		OrientedLabel::visit(visitor);
		visitor(position, "position")
		       (side, "side");
	}
};

struct AxisTitle : Label
{
	class SpecNameEnum(Position)
		(axis, min_edge, max_edge)
		(axis, min-edge, max-edge);
	
	class Enum(Side)(positive, upon, negative);
	class Enum(VPosition)(begin, middle, end);
	class Enum(VSide)(positive, upon, negative);
	class Enum(Orientation)(horizontal, vertical);

	Param<::Anim::Interpolated<Position>> position;
	Param<::Anim::Interpolated<Side>> side;
	Param<::Anim::Interpolated<VPosition>> vposition;
	Param<::Anim::Interpolated<VSide>> vside;
	Param<::Anim::Interpolated<Orientation>> orientation;

	void visit(auto &visitor)
	{
		Label::visit(visitor);
		visitor(position, "position")
		       (side, "side")
		       (vposition, "vposition")
		       (vside, "vside")
		       (orientation, "orientation");
	}
};

struct Axis
{
	Param<Gfx::Color> color;
	AxisTitle title;
	AxisLabel label;
	Tick ticks;
	Guide guides;
	Interlacing interlacing;

	void visit(auto &visitor)
	{
		visitor
			(color, "color")
			(title, "title")
			(label, "label")
			(ticks, "ticks")
			(guides, "guides")
			(interlacing, "interlacing");
	}
};

struct MarkerLabel : OrientedLabel
{
	class Enum(Position)(center, left, right, top, bottom);
	class Enum(Format)(measureFirst, dimensionsFirst);

	Param<::Anim::Interpolated<Position>> position;
	Param<Gfx::ColorTransform> filter;
	Param<Format> format;

	void visit(auto &visitor)
	{
		OrientedLabel::visit(visitor);
		visitor
			(position, "position")
			(filter, "filter")
			(format, "format");
	}
};

struct Tooltip : Font, Box
{
	class Enum(Layout)(singleLine, multiLine);

	Param<::Anim::Interpolated<Layout>> layout;
	Param<Gfx::Color> color;
	Param<Gfx::Color> shadowColor;
	Param<double> borderRadius;
	Param<double> dropShadow;
	Param<double> arrowSize;
	Param<double> distance;
	Param<::Anim::String> seriesName;

	void visit(auto &visitor) {
		Box::visit(visitor);
		Font::visit(visitor);
		visitor
			(layout, "layout")
			(color, "color")
			(shadowColor, "shadowColor")
			(borderRadius, "borderRadius")
			(dropShadow, "dropShadow")
			(arrowSize, "arrowSize")
			(distance, "distance")
			(seriesName, "seriesName");
	}
};

struct DataPoint
{
	Param<Gfx::ColorGradient> colorGradient;
	Param<Gfx::ColorPalette> colorPalette;
	Param<double> minLightness;
	Param<double> maxLightness;
	Param<double> lineWidth;
	Param<double> lineMinWidth;
	Param<double> lineMaxWidth;
	Param<double> circleMinRadius;
	Param<double> circleMaxRadius;
	Param<::Anim::Interpolated<std::optional<double>>> rectangleSpacing;

	Diag::ColorBuilder::LighnessRange lightnessRange() const
	{
		return { *minLightness, *maxLightness};
	}

	void visit(auto &visitor)
	{
		visitor
			(colorGradient, "colorGradient")
			(colorPalette, "colorPalette")
			(minLightness, "minLightness")
			(maxLightness, "maxLightness")
			(lineWidth, "lineWidth")
			(lineMinWidth, "lineMinWidth")
			(lineMaxWidth, "lineMaxWidth")
			(circleMinRadius, "circleMinRadius")
			(circleMaxRadius, "circleMaxRadius")
			(rectangleSpacing, "rectangleSpacing");
	}
};

struct Marker : DataPoint
{
	class Enum(BorderOpacityMode)(straight, premultiplied);

	Param<double> borderWidth;
	Param<double> borderOpacity;
	Param<Anim::Interpolated<BorderOpacityMode>> borderOpacityMode;
	Param<double> fillOpacity;
	Guide guides;
	MarkerLabel label;

	void visit(auto &visitor)
	{
		DataPoint::visit(visitor);

		visitor
			(borderWidth, "borderWidth")
			(borderOpacity, "borderOpacity")
			(borderOpacityMode, "borderOpacityMode")
			(fillOpacity, "fillOpacity")
			(guides, "guides")
			(label, "label");
	}
};

struct Legend : Padding, Box
{
	struct Marker {
		class Enum(Type)(circle, square);

		Param<::Anim::Interpolated<Type>> type;
		Param<Gfx::Length> size;

		void visit(auto &visitor)
		{
			visitor
				(type, "type")
				(size, "size");
		}
	};

	Param<Gfx::Length> width;
	Param<Gfx::Length> maxWidth;
	Label title;
	Label label;
	Marker marker;

	double computedWidth(double refSize, double fontSize) const {
		return std::min(
			width->get(refSize, fontSize),
			maxWidth->get(refSize, fontSize)
		);
	}

	void visit(auto &visitor)
	{
		Padding::visit(visitor);
		Box::visit(visitor);
		visitor
			(width, "width")
			(maxWidth, "maxWidth")
		    (title, "title")
		    (label, "label")
		    (marker, "marker");
	}
};

struct Plot : Padding, Box
{
	Marker marker;
	Axis xAxis;
	Axis yAxis;
	Param<Anim::Interpolated<Overflow>> overflow;

	const Axis &getAxis(Diag::ScaleId id) const {
		return id == Diag::ScaleId::x ? xAxis : yAxis;
	}

	void visit(auto &visitor)
	{
		Padding::visit(visitor);
		Box::visit(visitor);
		visitor
			(marker, "marker")
			(xAxis, "xAxis")
			(yAxis, "yAxis")
			(overflow, "overflow");
	}
};

struct Logo : Padding
{
	Param<Gfx::Length> width;
	Param<Gfx::ColorTransform> filter;

	void visit(auto &visitor)
	{
		Padding::visit(visitor);
		visitor
			(width, "width")
			(filter, "filter");
	}
};

struct Chart : Padding, Box, Font
{
	Plot plot;
	Legend legend;
	Label title;
	Tooltip tooltip;
	Logo logo;

	void visit(auto &visitor)
	{
		Padding::visit(visitor);
		Box::visit(visitor);
		Font::visit(visitor);
		visitor
			(plot, "plot")
			(legend, "legend")
			(title, "title")
			(tooltip, "tooltip")
			(logo, "logo");
	}

	static Font defaultFont;
	static Chart def();

	void setup() 
	{
		std::vector<Font*> fonts{
			&title,
			&plot.xAxis.title,
			&plot.xAxis.label,
			&plot.yAxis.title,
			&plot.yAxis.label,
			&plot.marker.label,
			&legend.title,
			&legend.label
		};
		fontParent = &defaultFont;
		for (auto font : fonts) font->fontParent = (Font*)this;
	}
};

}
}

#endif
