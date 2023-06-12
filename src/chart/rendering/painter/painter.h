#ifndef PAINTER_H
#define PAINTER_H

#include "base/gfx/canvas.h"

#include "coordinatesystem.h"
#include "painteroptions.h"

namespace Vizzu
{
namespace Draw
{

class IPainter;
typedef std::shared_ptr<IPainter> IPainterPtr;

class IPainter
{
public:
	virtual ~IPainter() {}

	virtual Gfx::ICanvas &getCanvas() = 0;

	virtual void setCoordSys(const CoordinateSystem &system) = 0;
	virtual void setResMode(const ResolutionMode &mode) = 0;

	virtual void drawLine(const Geom::Line &line) = 0;

	virtual void drawStraightLine(const Geom::Line &line,
	    std::array<double, 2> widths,
	    const Gfx::Color &endColor,
	    const Gfx::Color &lineColor) = 0;

	virtual void setPolygonToCircleFactor(double factor) = 0;
	virtual void setPolygonStraightFactor(double factor) = 0;

	virtual void drawPolygon(const std::array<Geom::Point, 4> &ps,
	    bool clip = false) = 0;
};

}
}

#endif
