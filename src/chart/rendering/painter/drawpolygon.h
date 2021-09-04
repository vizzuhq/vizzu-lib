#ifndef DRAW_H
#define DRAW_H

#include <array>
#include <list>
#include <vector>

#include "base/geom/point.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"

#include "pathsampler.h"

namespace Vizzu
{
namespace Draw
{

class drawPolygon
{
public:
	struct Options : PathSampler::Options
	{
		Options(CoordinateSystem &coordSys) :
			PathSampler::Options(coordSys),
			circ(0),
			linear(0)
		{}
		double circ;
		double linear;
	};

	drawPolygon(const std::array<Geom::Point, 4> &ps,
				const Options &options,
				Gfx::ICanvas &canvas);

private:

	class Path : public PathSampler
	{
	public:
		Path(const Geom::Point &p0,
		    const Geom::Point &p1,
		    Geom::Point center,
		    Geom::Point linSize,
		    Gfx::ICanvas &canvas,
		    const drawPolygon::Options &options);

		using Gfx::PathSampler::calc;
	private:
		const drawPolygon::Options &options;
		Gfx::ICanvas &canvas;
		Geom::Point centerConv;
		Geom::Point linP0;
		Geom::Point linP1;
		Geom::Size linSize;

		void addPoint(const Geom::Point &point) override;
		Geom::Point getPoint(double f) override;

		Geom::Point intpToElipse(Geom::Point point, double factor);

		Geom::Point projectToElipse(Geom::Point point);
	};

	Geom::Point center;
	Geom::Point boundary;
};

}
}

#endif
