#include "jscriptcanvas.h"

using namespace Vizzu;
using namespace Vizzu::Main;

extern "C" {
extern void canvas_textBoundary(const char *, double *, double *);
extern void canvas_setClipRect(double, double, double, double);
extern void canvas_setClipCircle(double, double, double);
extern void canvas_setClipPolygon();
extern void canvas_setBrushColor(double, double, double, double);
extern void canvas_setLineColor(double, double, double, double);
extern void canvas_setLineWidth(double);
extern void canvas_setFont(const char *);
extern void canvas_beginDropShadow();
extern void canvas_setDropShadowBlur(double);
extern void canvas_setDropShadowColor(double, double, double, double);
extern void canvas_setDropShadowOffset(double, double);
extern void canvas_endDropShadow();
extern void canvas_beginPolygon();
extern void canvas_addPoint(double, double);
extern void canvas_addBezier(double, double, double, double, double, double);
extern void canvas_endPolygon();
extern void canvas_rectangle(double, double, double, double);
extern void canvas_circle(double, double, double);
extern void canvas_line(double, double, double, double);
extern void canvas_text(double, double, double, double, const char *);
extern void canvas_setBrushGradient(double,
    double,
    double,
    double,
    size_t,
    const void *);
extern void canvas_frameBegin();
extern void canvas_frameEnd();
extern void
canvas_transform(double, double, double, double, double, double);
extern void canvas_save();
extern void canvas_restore();
}

Geom::Size JScriptCanvas::textBoundary(const std::string &text)
{
	Geom::Size res;
	::canvas_textBoundary(text.c_str(), &res.x, &res.y);
	return res;
}

Geom::Rect JScriptCanvas::getClipRect() const
{
	return clipRect ? *clipRect : Geom::Rect::CenteredMax();
}

void JScriptCanvas::setClipRect(const Geom::Rect &rect)
{
	if (!clipRect || *clipRect != rect) {
		clipRect = rect;
		::canvas_setClipRect(rect.pos.x,
		    rect.pos.y,
		    rect.size.x,
		    rect.size.y);
	}
}

void JScriptCanvas::setClipCircle(const Geom::Circle &circle)
{
	clipRect = circle.boundary();
	::canvas_setClipCircle(circle.center.x,
	    circle.center.y,
	    circle.radius);
}

void JScriptCanvas::setClipPolygon()
{
	::canvas_setClipPolygon();
}

void JScriptCanvas::setBrushColor(const Gfx::Color &color)
{
	if (color != brushColor) {
		brushColor = color;
		::canvas_setBrushColor(color.red,
		    color.green,
		    color.blue,
		    color.alpha);
	}
}

void JScriptCanvas::setLineColor(const Gfx::Color &color)
{
	if (color != lineColor)
		::canvas_setLineColor(color.red,
		    color.green,
		    color.blue,
		    color.alpha);
}

void JScriptCanvas::setLineWidth(double width)
{
	if (width != lineWidth) {
		lineWidth = width;
		::canvas_setLineWidth(width);
	}
}

void JScriptCanvas::setFont(const Gfx::Font &font)
{
	if (this->font != font) {
		this->font = font;
		auto cssFont = font.toCSS();
		::canvas_setFont(cssFont.c_str());
	}
}

void JScriptCanvas::setTextColor(const Gfx::Color &color)
{
	if (color != brushColor) {
		brushColor = color;
		::canvas_setBrushColor(color.red,
		    color.green,
		    color.blue,
		    color.alpha);
	}
}

void JScriptCanvas::beginDropShadow()
{
	::canvas_beginDropShadow();
}

void JScriptCanvas::setDropShadowBlur(uint64_t radius)
{
	::canvas_setDropShadowBlur(radius);
}

void JScriptCanvas::setDropShadowColor(const Gfx::Color &color)
{
	::canvas_setDropShadowColor(color.red,
	    color.green,
	    color.blue,
	    color.alpha);
}

void JScriptCanvas::setDropShadowOffset(
    const Geom::Point &offset)
{
	::canvas_setDropShadowOffset(offset.x, offset.y);
}

void JScriptCanvas::endDropShadow()
{
	::canvas_endDropShadow();
}

void JScriptCanvas::beginPolygon()
{
	::canvas_beginPolygon();
}

void JScriptCanvas::addPoint(const Geom::Point &point)
{
	::canvas_addPoint(point.x, point.y);
}

void JScriptCanvas::addBezier(const Geom::Point &control0,
    const Geom::Point &control1,
    const Geom::Point &endPoint)
{
	canvas_addBezier(control0.x,
	    control0.y,
	    control1.x,
	    control1.y,
	    endPoint.x,
	    endPoint.y);
}

void JScriptCanvas::endPolygon()
{
	::canvas_endPolygon();
}

void JScriptCanvas::rectangle(const Geom::Rect &rect)
{
	::canvas_rectangle(rect.pos.x,
	    rect.pos.y,
	    rect.size.x,
	    rect.size.y);
}

void JScriptCanvas::circle(const Geom::Circle &circle)
{
	::canvas_circle(circle.center.x, circle.center.y, circle.radius);
}

void JScriptCanvas::line(const Geom::Line &line)
{
	::canvas_line(line.begin.x, line.begin.y, line.end.x, line.end.y);
}

void JScriptCanvas::text(const Geom::Rect &rect,
    const std::string &str)
{
	::canvas_text(rect.pos.x,
	    rect.pos.y,
	    rect.size.x,
	    rect.size.y,
	    str.c_str());
}

void JScriptCanvas::setBrushGradient(const Geom::Line &line,
    const Gfx::ColorGradient &gradient)
{
	typedef decltype(gradient.stops)::value_type Stop;
	static_assert(sizeof(Stop) == sizeof(double) * 5);
	
	static_assert(offsetof(Stop, pos) == 0);
	static_assert(std::is_same<decltype(Stop::pos), double>::value);

	static_assert(offsetof(Stop, value) == sizeof(double));
	static_assert(std::is_same<decltype(Stop::value.red), double>::value);
	static_assert(std::is_same<decltype(Stop::value.green), double>::value);
	static_assert(std::is_same<decltype(Stop::value.blue), double>::value);
	static_assert(std::is_same<decltype(Stop::value.alpha), double>::value);
	
	::canvas_setBrushGradient(line.begin.x,
	    line.begin.y,
	    line.end.x,
	    line.end.y,
	    gradient.stops.size(),
	    gradient.stops.data());
}

void JScriptCanvas::frameEnd()
{
	::canvas_frameEnd();
}

void JScriptCanvas::frameBegin()
{
	resetStates();
	::canvas_frameBegin();
}

void JScriptCanvas::transform(
    const Geom::AffineTransform &transform)
{
	const auto &[r0, r1] = transform.getMatrix();
	::canvas_transform(r0[0], r1[0], r0[1], r1[1], r0[2], r1[2]);
}

void JScriptCanvas::save()
{
	::canvas_save();
}

void JScriptCanvas::restore()
{
	::canvas_restore();
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
