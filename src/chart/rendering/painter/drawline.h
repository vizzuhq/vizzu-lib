#ifndef DRAWLINE_H
#define DRAWLINE_H

#include "base/geom/line.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"

#include "coordinatesystem.h"
#include "painteroptions.h"
#include "pathsampler.h"

namespace Vizzu::Draw
{

class DrawLine
{
public:
	DrawLine(const Geom::Line &line,
	    ResolutionMode resolutionMode,
	    CoordinateSystem &coordSys,
	    Gfx::ICanvas &canvas);

	DrawLine(const Geom::Line &line,
	    std::array<double, 2> widths,
	    double straightFactor,
	    CoordinateSystem &coordSys,
	    Gfx::ICanvas &canvas);

private:
	class Path : public PathSampler
	{
	public:
		Path(const Geom::Point &p0,
		    const Geom::Point &p1,
		    Gfx::ICanvas &canvas,
		    const Options &options);

		void sample();

	private:
		Gfx::ICanvas &canvas;
		Geom::Point lastPoint;
		void addPoint(const Geom::Point &point) override;
		Geom::Point getPoint(double i) override;
	};
};

}

#endif
