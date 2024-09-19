#ifndef DRAWLINE_H
#define DRAWLINE_H

#include "base/geom/line.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"

#include "coordinatesystem.h"
#include "pathsampler.h"

namespace Vizzu::Draw
{

class DrawLine
{
public:
	struct LineOptions
	{
		double straightFactor;
	};

	struct Options : PathSampler::Options, LineOptions
	{};

	DrawLine(const Geom::Line &line,
	    const PathSampler::Options &options,
	    Gfx::ICanvas &canvas);

	DrawLine(const Geom::Line &line,
	    std::array<double, 2> widths,
	    const Options &options,
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
