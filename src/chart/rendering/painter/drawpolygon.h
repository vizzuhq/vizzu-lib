#ifndef DRAWPOLYGON_H
#define DRAWPOLYGON_H

#include <array>
#include <vector>

#include "base/geom/point.h"
#include "base/gfx/canvas.h"
#include "base/gfx/color.h"

#include "pathsampler.h"

namespace Vizzu::Draw
{

class DrawPolygon
{
public:
	struct PolygonOptions
	{
		double toCircleFactor;
		double straightFactor;
	};

	struct Options : PathSampler::Options, PolygonOptions
	{};

	DrawPolygon(const std::array<Geom::Point, 4> &ps,
	    const Options &options,
	    Gfx::ICanvas &canvas,
	    bool clip);

private:
	class Path : public PathSampler
	{
	public:
		Path(const Geom::Point &p0,
		    const Geom::Point &p1,
		    Geom::Point center,
		    Geom::Point linSize,
		    Gfx::ICanvas &canvas,
		    const DrawPolygon::Options &options);

		using Gfx::PathSampler::calc;

	private:
		const DrawPolygon::Options &options;
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

#endif
