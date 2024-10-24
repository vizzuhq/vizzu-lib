#include "jscriptcanvas.h"

#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>

#include "base/geom/affinetransform.h"
#include "base/geom/circle.h"
#include "base/geom/line.h"
#include "base/geom/rect.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/font.h"

#include "canvas.h"
#include "interfacejs.h"

namespace Vizzu::Main
{

void JScriptCanvas::setClipRect(const Geom::Rect &rect)
{
	if (!clipRect || *clipRect != rect) {
		clipRect = rect;
		::canvas_setClipRect(this,
		    rect.pos.x,
		    rect.pos.y,
		    rect.size.x,
		    rect.size.y);
	}
}

void JScriptCanvas::setClipCircle(const Geom::Circle &circle)
{
	clipRect = circle.boundary();
	::canvas_setClipCircle(this,
	    circle.center.x,
	    circle.center.y,
	    circle.radius);
}

void JScriptCanvas::setClipPolygon()
{
	::canvas_setClipPolygon(this);
}

void JScriptCanvas::setBrushColor(const Gfx::Color &color)
{
	if (color != brushColor) {
		brushColor = color;
		::canvas_setBrushColor(this,
		    color.red,
		    color.green,
		    color.blue,
		    color.alpha);
	}
}

void JScriptCanvas::setLineColor(const Gfx::Color &color)
{
	if (color != lineColor)
		::canvas_setLineColor(this,
		    color.red,
		    color.green,
		    color.blue,
		    color.alpha);
}

void JScriptCanvas::setLineWidth(double width)
{
	if (width != lineWidth) {
		lineWidth = width;
		::canvas_setLineWidth(this, width);
	}
}

void JScriptCanvas::setFont(const Gfx::Font &font)
{
	if (this->font != font) {
		this->font = font;
		auto cssFont = font.toCSS();
		::canvas_setFont(this, cssFont.c_str());
	}
}

void JScriptCanvas::beginDropShadow()
{
	::canvas_beginDropShadow(this);
}

void JScriptCanvas::setDropShadowBlur(double radius)
{
	::canvas_setDropShadowBlur(this, radius);
}

void JScriptCanvas::setDropShadowColor(const Gfx::Color &color)
{
	::canvas_setDropShadowColor(this,
	    color.red,
	    color.green,
	    color.blue,
	    color.alpha);
}

void JScriptCanvas::setDropShadowOffset(const Geom::Point &offset)
{
	::canvas_setDropShadowOffset(this, offset.x, offset.y);
}

void JScriptCanvas::endDropShadow() { ::canvas_endDropShadow(this); }

void JScriptCanvas::beginPolygon() { ::canvas_beginPolygon(this); }

void JScriptCanvas::addPoint(const Geom::Point &point)
{
	::canvas_addPoint(this, point.x, point.y);
}

void JScriptCanvas::addBezier(const Geom::Point &control0,
    const Geom::Point &control1,
    const Geom::Point &endPoint)
{
	canvas_addBezier(this,
	    control0.x,
	    control0.y,
	    control1.x,
	    control1.y,
	    endPoint.x,
	    endPoint.y);
}

void JScriptCanvas::endPolygon() { ::canvas_endPolygon(this); }

void JScriptCanvas::rectangle(const Geom::Rect &rect)
{
	::canvas_rectangle(this,
	    rect.pos.x,
	    rect.pos.y,
	    rect.size.x,
	    rect.size.y);
}

void JScriptCanvas::circle(const Geom::Circle &circle)
{
	::canvas_circle(this,
	    circle.center.x,
	    circle.center.y,
	    circle.radius);
}

void JScriptCanvas::line(const Geom::Line &line)
{
	::canvas_line(this,
	    line.begin.x,
	    line.begin.y,
	    line.end.x,
	    line.end.y);
}

void JScriptCanvas::text(const Geom::Rect &rect,
    const std::string &text)
{
	::canvas_text(this,
	    rect.pos.x,
	    rect.pos.y,
	    rect.size.x,
	    rect.size.y,
	    text.c_str());
}

void JScriptCanvas::setBrushGradient(
    const Gfx::LinearGradient &gradient)
{
	typedef decltype(gradient.colors.stops)::value_type Stop;
	static_assert(sizeof(double) == 8);
	static_assert(sizeof(Stop) == sizeof(double) * 5);

	static_assert(offsetof(Stop, pos) == 0);
	static_assert(std::is_same_v<decltype(Stop::pos), double>);

	static_assert(offsetof(Stop, value) == sizeof(double));
	static_assert(std::is_same_v<decltype(Stop::value.red), double>);
	static_assert(
	    std::is_same_v<decltype(Stop::value.green), double>);
	static_assert(std::is_same_v<decltype(Stop::value.blue), double>);
	static_assert(
	    std::is_same_v<decltype(Stop::value.alpha), double>);

	::canvas_setBrushGradient(this,
	    gradient.line.begin.x,
	    gradient.line.begin.y,
	    gradient.line.end.x,
	    gradient.line.end.y,
	    gradient.colors.stops.size(),
	    gradient.colors.stops.data());
}

void JScriptCanvas::frameEnd() { ::canvas_frameEnd(this); }

void JScriptCanvas::frameBegin()
{
	resetStates();
	::canvas_frameBegin(this);
}

void JScriptCanvas::transform(const Geom::AffineTransform &transform)
{
	const auto &[r0, r1] = transform.getMatrix();
	::canvas_transform(this,
	    r0[0],
	    r1[0],
	    r0[1],
	    r1[1],
	    r0[2],
	    r1[2]);
}

void JScriptCanvas::save() { ::canvas_save(this); }

void JScriptCanvas::restore()
{
	::canvas_restore(this);
	resetStates();
}

void JScriptCanvas::resetStates()
{
	font = std::nullopt;
	brushColor = std::nullopt;
	lineColor = std::nullopt;
	lineWidth = std::nullopt;
	clipRect = std::nullopt;
}

}

Geom::Size Gfx::ICanvas::textBoundary(const Gfx::Font &font,
    const std::string &text)
{
	Geom::Size res;
	::textBoundary(font.toCSS().c_str(),
	    text.c_str(),
	    &res.x,
	    &res.y);
	return res;
}