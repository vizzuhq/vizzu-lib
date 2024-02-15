#ifndef PAINTER_H
#define PAINTER_H

#include "base/gfx/canvas.h"

#include "coordinatesystem.h"
#include "painteroptions.h"

namespace Vizzu::Draw
{

class Painter
{
public:
	virtual ~Painter() = default;

	virtual Gfx::ICanvas &getCanvas() = 0;

	void setCoordSys(const CoordinateSystem &system)
	{
		this->system = system;
	}

	void setResMode(const ResolutionMode &mode) { this->mode = mode; }

	void drawLine(const Geom::Line &line);

	void drawStraightLine(const Geom::Line &line,
	    std::array<double, 2> widths,
	    double straightFactor);

	void setPolygonToCircleFactor(double factor)
	{
		polygonOptions.toCircleFactor = factor;
	}
	void setPolygonStraightFactor(double factor)
	{
		polygonOptions.straightFactor = factor;
	}

	void drawPolygon(const std::array<Geom::Point, 4> &ps,
	    bool clip = false);

private:
	struct PolygonOptions
	{
		double toCircleFactor;
		double straightFactor;
	};

	CoordinateSystem system;
	ResolutionMode mode{ResolutionMode::Low};
	PolygonOptions polygonOptions{};
};

}

#endif
