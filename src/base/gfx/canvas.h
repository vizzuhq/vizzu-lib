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
#include "base/gfx/pixmap.h"
#include "base/gfx/svg.h"

#define MEASURE_CANVAS_RUNTIME 0

namespace Gfx
{

struct ICanvas;
typedef std::shared_ptr<ICanvas> ICanvasPtr;

struct ICanvas
{
	virtual ~ICanvas() {}

	// todo: Remove this function and move to a dedicated canvas
	// factory.
	virtual ICanvasPtr createCanvas(int width, int heigth) = 0;

	virtual Geom::Size textBoundary(const std::string &string) = 0;
	virtual Geom::Rect getClipRect() const = 0;
	virtual void setClipRect(const Geom::Rect &rect) = 0;
	virtual void setClipPolygon() = 0;
	virtual void setBrushColor(const Gfx::Color &color) = 0;
	virtual void setLineColor(const Gfx::Color &color) = 0;
	virtual void setTextColor(const Gfx::Color &color) = 0;
	virtual void setLineWidth(double width) = 0;
	virtual void setFont(const Gfx::Font &font) = 0;

	virtual void transform(const Geom::AffineTransform &transform) = 0;
	virtual void save() = 0;
	virtual void restore() = 0;

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

	virtual int loadSvgImage(const Gfx::Svg &svg) = 0;
	virtual int loadPixMapImage(const Gfx::PixMapView &pixmap) = 0;
	virtual void drawImage(int imageId,
						   const Geom::Rect &boundary,
						   double opacity = 1.0) = 0;
	virtual void dropImage(int imageId = -1) = 0;

	virtual void drawCanvas(const Geom::Rect &rect, const ICanvas &canvas) = 0;

	virtual void frameBegin() = 0;
	virtual void frameEnd() = 0;
};

struct ICanvasBuilder
{
	virtual ~ICanvasBuilder() {}
	virtual ICanvasPtr getCanvas() = 0;
	virtual void releaseCanvas() = 0;
};

typedef std::shared_ptr<ICanvasBuilder> ICanvasBuilderPtr;

}

#endif

