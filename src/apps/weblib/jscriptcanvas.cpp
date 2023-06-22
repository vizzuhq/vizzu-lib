#include "jscriptcanvas.h"

#include "base/util/stopwatch.h"

using namespace Vizzu;
using namespace Vizzu::Main;

struct CanvasRuntime : public ElapsedTime<CanvasRuntime>
{
	using ElapsedTime::ElapsedTime;
};

#if MEASURE_CANVAS_RUNTIME == 1
#define _measure_runtime(C) Stopwatch<C> __(__func__);
#else
#define _measure_runtime(C)
#endif

extern "C" {
extern void canvas_textBoundary(const char *, double *, double *);
extern void canvas_setClipRect(double, double, double, double);
extern void canvas_setClipCircle(double, double, double);
extern void canvas_setClipPolygon();
extern void canvas_setBrushColor(double, double, double, double);
extern void canvas_setLineColor(double, double, double, double);
extern void canvas_setLineWidth(double);
extern void canvas_setFont(const char *);
extern void canvas_setTextColor(double, double, double, double);
extern void canvas_beginDropShadow();
extern void canvas_setDropShadowBlur(double);
extern void canvas_setDropShadowColor(double, double, double, double);
extern void canvas_setDropShadowOffset(double, double);
extern void canvas_endDropShadow();
extern void canvas_beginPolygon();
extern void canvas_addPoint(double, double);
extern void
canvas_addBezier(double, double, double, double, double, double);
extern void canvas_endPolygon();
extern void canvas_rectangle(double, double, double, double);
extern void canvas_circle(double, double, double);
extern void canvas_line(double, double, double, double);
extern void canvas_text(double, double, double, double, const char *);
extern void canvas_setBrushGradient(double,
    double,
    double,
    double,
    int,
    const char *);
extern void canvas_frameBegin();
extern void canvas_frameEnd();
extern void
canvas_transform(double, double, double, double, double, double);
extern void canvas_save();
extern void canvas_restore();
}

JScriptCanvas::JScriptCanvas() { CanvasRuntime::start(); }

JScriptCanvas::~JScriptCanvas() { CanvasRuntime::stop(); }

Geom::Size JScriptCanvas::textBoundary(const std::string &text)
{
	_measure_runtime(CanvasRuntime);
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
	_measure_runtime(CanvasRuntime);
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
	_measure_runtime(CanvasRuntime);
	clipRect = circle.boundary();
	::canvas_setClipCircle(circle.center.x,
	    circle.center.y,
	    circle.radius);
}

void JScriptCanvas::setClipPolygon()
{
	_measure_runtime(CanvasRuntime);
	::canvas_setClipPolygon();
}

void JScriptCanvas::setBrushColor(const Gfx::Color &color)
{
	_measure_runtime(CanvasRuntime);
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
	_measure_runtime(CanvasRuntime);
	if (color != lineColor)
		::canvas_setLineColor(color.red,
		    color.green,
		    color.blue,
		    color.alpha);
}

void JScriptCanvas::setLineWidth(double width)
{
	_measure_runtime(CanvasRuntime);
	if (width != lineWidth) {
		lineWidth = width;
		::canvas_setLineWidth(width);
	}
}

void JScriptCanvas::setFont(const Gfx::Font &font)
{
	_measure_runtime(CanvasRuntime);
	if (this->font != font) {
		this->font = font;
		auto cssFont = font.toCSS();
		::canvas_setFont(cssFont.c_str());
	}
}

void JScriptCanvas::setTextColor(const Gfx::Color &color)
{
	_measure_runtime(CanvasRuntime);
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
	_measure_runtime(CanvasRuntime);
	::canvas_beginDropShadow();
}

void JScriptCanvas::setDropShadowBlur(uint64_t radius)
{
	_measure_runtime(CanvasRuntime);
	::canvas_setDropShadowBlur(radius);
}

void JScriptCanvas::setDropShadowColor(const Gfx::Color &color)
{
	_measure_runtime(CanvasRuntime);
	::canvas_setDropShadowColor(color.red,
	    color.green,
	    color.blue,
	    color.alpha);
}

void JScriptCanvas::setDropShadowOffset(
    const Geom::Point &offset)
{
	_measure_runtime(CanvasRuntime);
	::canvas_setDropShadowOffset(offset.x, offset.y);
}

void JScriptCanvas::endDropShadow()
{
	_measure_runtime(CanvasRuntime);
	::canvas_endDropShadow();
}

void JScriptCanvas::beginPolygon()
{
	_measure_runtime(CanvasRuntime);
	::canvas_beginPolygon();
}

void JScriptCanvas::addPoint(const Geom::Point &point)
{
	_measure_runtime(CanvasRuntime);
	::canvas_addPoint(point.x, point.y);
}

void JScriptCanvas::addBezier(const Geom::Point &control0,
    const Geom::Point &control1,
    const Geom::Point &endPoint)
{
	_measure_runtime(CanvasRuntime);
	canvas_addBezier(control0.x,
	    control0.y,
	    control1.x,
	    control1.y,
	    endPoint.x,
	    endPoint.y);
}

void JScriptCanvas::endPolygon()
{
	_measure_runtime(CanvasRuntime);
	::canvas_endPolygon();
}

void JScriptCanvas::rectangle(const Geom::Rect &rect)
{
	_measure_runtime(CanvasRuntime);
	::canvas_rectangle(rect.pos.x,
	    rect.pos.y,
	    rect.size.x,
	    rect.size.y);
}

void JScriptCanvas::circle(const Geom::Circle &circle)
{
	_measure_runtime(CanvasRuntime);
	::canvas_circle(circle.center.x, circle.center.y, circle.radius);
}

void JScriptCanvas::line(const Geom::Line &line)
{
	_measure_runtime(CanvasRuntime);
	::canvas_line(line.begin.x, line.begin.y, line.end.x, line.end.y);
}

void JScriptCanvas::text(const Geom::Rect &rect,
    const std::string &str)
{
	_measure_runtime(CanvasRuntime);
	::canvas_text(rect.pos.x,
	    rect.pos.y,
	    rect.size.x,
	    rect.size.y,
	    str.c_str());
}

void JScriptCanvas::setBrushGradient(const Geom::Line &line,
    const Gfx::ColorGradient &gradient)
{
	_measure_runtime(CanvasRuntime);
	::canvas_setBrushGradient(line.begin.x,
	    line.begin.y,
	    line.end.x,
	    line.end.y,
	    (int)gradient.stops.size(),
	    (char *)gradient.stops.data());
}

void JScriptCanvas::frameEnd()
{
	_measure_runtime(CanvasRuntime);
	::canvas_frameEnd();
}

void JScriptCanvas::frameBegin()
{
	_measure_runtime(CanvasRuntime);
	resetStates();
	::canvas_frameBegin();
}

void JScriptCanvas::transform(
    const Geom::AffineTransform &transform)
{
	_measure_runtime(CanvasRuntime);
	const auto &[r0, r1] = transform.getMatrix();
	::canvas_transform(r0[0], r1[0], r0[1], r1[1], r0[2], r1[2]);
}

void JScriptCanvas::save()
{
	_measure_runtime(CanvasRuntime);
	::canvas_save();
}

void JScriptCanvas::restore()
{
	_measure_runtime(CanvasRuntime);
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
