#ifndef PAINTER_H
#define PAINTER_H

#include "base/gfx/canvas.h"
#include "base/gfx/pathsampler.h"

#include "coordinatesystem.h"
#include "drawpolygon.h"

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

	void setPathSamplerOptions(
	    const Gfx::PathSampler::Options &options)
	{
		pathSamplerOptions = options;
	}

private:
	CoordinateSystem system;
	Gfx::PathSampler::Options pathSamplerOptions{1.0, 0.5};
	DrawPolygon::PolygonOptions polygonOptions{};
};

}

#endif
