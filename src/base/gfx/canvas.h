#ifndef GFX_CANVAS
#define GFX_CANVAS

#include <memory>
#include <string>

#include "base/geom/affinetransform.h"
#include "base/geom/circle.h"
#include "base/geom/line.h"
#include "base/geom/point.h"
#include "base/geom/rect.h"
#include "base/gfx/color.h"
#include "base/gfx/colorgradient.h"
#include "base/gfx/colortransform.h"
#include "base/gfx/font.h"

namespace Gfx
{

struct ICanvas;

struct ICanvas
{
	virtual ~ICanvas() = default;

	virtual void setClipRect(const Geom::Rect &rect) = 0;
	virtual void setClipCircle(const Geom::Circle &circle) = 0;
	virtual void setClipPolygon() = 0;
	virtual void setBrushColor(const Gfx::Color &color) = 0;
	virtual void setLineColor(const Gfx::Color &color) = 0;
	virtual void setTextColor(const Gfx::Color &color) = 0;
	virtual void setLineWidth(double width) = 0;
	virtual void setFont(const Gfx::Font &font) = 0;

	virtual void transform(
	    const Geom::AffineTransform &transform) = 0;
	virtual void save() = 0;
	virtual void restore() = 0;
	virtual void beginDropShadow() = 0;
	virtual void setDropShadowBlur(double radius) = 0;
	virtual void setDropShadowColor(const Gfx::Color &color) = 0;
	virtual void setDropShadowOffset(const Geom::Point &offset) = 0;
	virtual void endDropShadow() = 0;

	virtual void beginPolygon() = 0;
	virtual void addPoint(const Geom::Point &point) = 0;
	virtual void addBezier(const Geom::Point &control0,
	    const Geom::Point &control1,
	    const Geom::Point &endPoint) = 0;
	virtual void endPolygon() = 0;

	virtual void rectangle(const Geom::Rect &rect) = 0;
	virtual void circle(const Geom::Circle &circle) = 0;
	virtual void line(const Geom::Line &line) = 0;

	virtual void text(const Geom::Rect &rect,
	    const std::string &text) = 0;

	virtual void setBrushGradient(const Geom::Line &line,
	    const ColorGradient &gradient) = 0;

	virtual void frameBegin() = 0;
	virtual void frameEnd() = 0;

	virtual void *getPainter() = 0;

	static Geom::Size textBoundary(const Gfx::Font &,
	    const std::string &);
};

}

#endif
