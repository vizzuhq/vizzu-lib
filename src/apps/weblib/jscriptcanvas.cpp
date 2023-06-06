#include "base/util/stopwatch.h"
#include "jscriptcanvas.h"

using namespace Vizzu;
using namespace Vizzu::Main;

struct CanvasRuntime : public ElapsedTime<CanvasRuntime> {
	using ElapsedTime::ElapsedTime;
};

#if MEASURE_CANVAS_RUNTIME == 1
#define _measure_runtime(C) \
    Stopwatch<C> __(__func__);
#else
#define _measure_runtime(C)
#endif

extern "C" {
    extern void canvas_textBoundary(const char*, double*, double*);
    extern void canvas_setClipRect(double, double, double, double);
	extern void canvas_setClipCircle(double, double, double);
	extern void canvas_setClipPolygon();
    extern void canvas_setBrushColor(double, double, double, double);
    extern void canvas_setLineColor(double, double, double, double);
    extern void canvas_setLineWidth(double);
	extern void canvas_setFont(const char*);
    extern void canvas_setTextColor(double, double, double, double);
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
    extern void canvas_text(double, double, double, double, const char*);
    extern void canvas_setBrushGradient(double, double, double, double, int, const char*);
	extern int canvas_loadSvgImage(const char*);
	extern int canvas_loadPixMapImage(const char*);
    extern void canvas_drawImage(int, double, double, double, double, double);
	extern void canvas_dropImage(int);
	extern void canvas_frameBegin();
    extern void canvas_frameEnd();
	extern void canvas_transform(double, double, double, double, double, double);
    extern void canvas_save();
    extern void canvas_restore();
}

JScriptOutputCanvas::JScriptOutputCanvas() {
	CanvasRuntime::start();
}

JScriptOutputCanvas::~JScriptOutputCanvas() {
	CanvasRuntime::stop();
}

std::shared_ptr<Gfx::ICanvas> JScriptOutputCanvas::createCanvas(int, int)  {
	return std::shared_ptr<JScriptOutputCanvas>();
}

Geom::Size JScriptOutputCanvas::textBoundary(const std::string &text)
{
	_measure_runtime(CanvasRuntime);
	Geom::Size res;
	::canvas_textBoundary(text.c_str(), &res.x, &res.y);
	return res;
}

Geom::Rect JScriptOutputCanvas::getClipRect() const {
	return clipRect ? *clipRect : Geom::Rect::CenteredMax();
}

void JScriptOutputCanvas::setClipRect(const Geom::Rect &rect) 
{
	_measure_runtime(CanvasRuntime);
	if (!clipRect || *clipRect != rect) {
		clipRect = rect;
		::canvas_setClipRect(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
	}
}

void JScriptOutputCanvas::setClipCircle(const Geom::Circle &circle)
{
	_measure_runtime(CanvasRuntime);
	clipRect = circle.boundary();
	::canvas_setClipCircle(circle.center.x, circle.center.y, circle.radius);
}

void JScriptOutputCanvas::setClipPolygon() {
	_measure_runtime(CanvasRuntime);
	::canvas_setClipPolygon();
}

void JScriptOutputCanvas::setBrushColor(const Gfx::Color &color) {
	_measure_runtime(CanvasRuntime);
	if (color != brushColor) {
		brushColor = color;
		::canvas_setBrushColor(color.red, color.green, color.blue, color.alpha);
	}
}

void JScriptOutputCanvas::setLineColor(const Gfx::Color &color) {
	_measure_runtime(CanvasRuntime);
	if (color != lineColor)
		::canvas_setLineColor(color.red, color.green, color.blue, color.alpha);
}

void JScriptOutputCanvas::setLineWidth(double width) {
	_measure_runtime(CanvasRuntime);
	if (width != lineWidth) {
		lineWidth = width;
		::canvas_setLineWidth(width);
	}
}

void JScriptOutputCanvas::setFont(const Gfx::Font &font) {
	_measure_runtime(CanvasRuntime);
	if (this->font != font)
	{
		this->font = font;
		auto cssFont = font.toCSS();
		::canvas_setFont(cssFont.c_str());
	}
}

void JScriptOutputCanvas::setTextColor(const Gfx::Color &color) {
	_measure_runtime(CanvasRuntime);
	if (color != brushColor) {
		brushColor = color;
		::canvas_setBrushColor(color.red, color.green, color.blue, color.alpha);
	}
}

void JScriptOutputCanvas::beginDropShadow() {
	_measure_runtime(CanvasRuntime);
	::canvas_beginDropShadow();
}

void JScriptOutputCanvas::setDropShadowBlur(uint64_t radius) {
	_measure_runtime(CanvasRuntime);
	::canvas_setDropShadowBlur(radius);
}

void JScriptOutputCanvas::setDropShadowColor(const Gfx::Color &color) {
	_measure_runtime(CanvasRuntime);
	::canvas_setDropShadowColor(color.red, color.green, color.blue, color.alpha);
}

void JScriptOutputCanvas::setDropShadowOffset(const Geom::Point &offset) {
	_measure_runtime(CanvasRuntime);
	::canvas_setDropShadowOffset(offset.x, offset.y);
}

void JScriptOutputCanvas::endDropShadow() {
	_measure_runtime(CanvasRuntime);
	::canvas_endDropShadow();
}

void JScriptOutputCanvas::beginPolygon() {
	_measure_runtime(CanvasRuntime);
	::canvas_beginPolygon();
}

void JScriptOutputCanvas::addPoint(const Geom::Point& point) {
	_measure_runtime(CanvasRuntime);
	::canvas_addPoint(point.x, point.y);
}

void JScriptOutputCanvas::addBezier(const Geom::Point &control0,
    const Geom::Point &control1,
    const Geom::Point &endPoint)
{
	_measure_runtime(CanvasRuntime);
	canvas_addBezier(
		control0.x, control0.y,
		control1.x, control1.y,
		endPoint.x, endPoint.y);
}

void JScriptOutputCanvas::endPolygon() {
	_measure_runtime(CanvasRuntime);
	::canvas_endPolygon();
}

void JScriptOutputCanvas::rectangle(const Geom::Rect& rect) {
	_measure_runtime(CanvasRuntime);
	::canvas_rectangle(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y);
}

void JScriptOutputCanvas::circle(const Geom::Circle& circle) {
	_measure_runtime(CanvasRuntime);
	::canvas_circle(circle.center.x, circle.center.y, circle.radius);
}

void JScriptOutputCanvas::line(const Geom::Line& line) {
	_measure_runtime(CanvasRuntime);
	::canvas_line(line.begin.x, line.begin.y, line.end.x, line.end.y);
}

void JScriptOutputCanvas::text(const Geom::Rect& rect,
	const std::string &str)
{
	_measure_runtime(CanvasRuntime);
	::canvas_text(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y,
		str.c_str());
}

void JScriptOutputCanvas::setBrushGradient(
	const Geom::Line &line, const Gfx::ColorGradient &gradient)
{
	_measure_runtime(CanvasRuntime);
	::canvas_setBrushGradient(line.begin.x, line.begin.y, line.end.x, line.end.y,
		(int)gradient.stops.size(), (char*)gradient.stops.data());
}

int JScriptOutputCanvas::loadSvgImage(const Gfx::Svg &) {
	_measure_runtime(CanvasRuntime);
	return 0;
}

int JScriptOutputCanvas::loadPixMapImage(const Gfx::PixMapView &) {
	_measure_runtime(CanvasRuntime);
	return 0;
}

void JScriptOutputCanvas::drawImage(int imageId, const Geom::Rect& rect, double opacity)
{
	_measure_runtime(CanvasRuntime);
	::canvas_drawImage(imageId, rect.pos.x, rect.pos.y, rect.size.x, rect.size.y, opacity);
}

void JScriptOutputCanvas::dropImage(int) {
	_measure_runtime(CanvasRuntime);
}

void JScriptOutputCanvas::drawCanvas(
	const Geom::Rect&, const ICanvas&)
{
    _measure_runtime(CanvasRuntime);
}

void JScriptOutputCanvas::frameEnd() {
	_measure_runtime(CanvasRuntime);
	::canvas_frameEnd();
}

void JScriptOutputCanvas::frameBegin() {
	_measure_runtime(CanvasRuntime);
	resetStates();
	::canvas_frameBegin();
}

void JScriptOutputCanvas::transform(const Geom::AffineTransform &transform) {
	_measure_runtime(CanvasRuntime);
 	const auto& [r0, r1] = transform.getMatrix();
	::canvas_transform(r0[0], r1[0], r0[1], r1[1], r0[2], r1[2]);
}

void JScriptOutputCanvas::save() {
	_measure_runtime(CanvasRuntime);
	::canvas_save();
}

void JScriptOutputCanvas::restore() {
	_measure_runtime(CanvasRuntime);
	::canvas_restore();
	resetStates();
}

void JScriptOutputCanvas::resetStates()
{
	font = std::nullopt;
	brushColor = std::nullopt;
	lineColor = std::nullopt;
	lineWidth = std::nullopt;
	clipRect = std::nullopt;
}
