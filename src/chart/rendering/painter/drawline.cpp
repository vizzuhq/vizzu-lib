#include "drawline.h"

#include <array>

#include "base/geom/line.h"
#include "base/geom/quadrilateral.h"
#include "base/gfx/canvas.h"
#include "base/math/interpolation.h"

#include "coordinatesystem.h"
#include "drawpolygon.h"
#include "pathsampler.h"

namespace Vizzu::Draw
{

DrawLine::DrawLine(const Geom::Line &line,
    const PathSampler::Options &options,
    Gfx::ICanvas &canvas)
{
	Path(line.begin, line.end, canvas, options).sample();
}

DrawLine::DrawLine(const Geom::Line &line,
    std::array<double, 2> widths,
    const Options &options,
    Gfx::ICanvas &canvas)
{
	auto pBeg = options.coordSys.convert(line.begin);
	auto pEnd = options.coordSys.convert(line.end);

	auto wBeg = widths[0] * options.coordSys.getRect().size.minSize();
	auto wEnd = widths[1] * options.coordSys.getRect().size.minSize();

	const auto &[p0, p1, p2, p3] =
	    Geom::ConvexQuad::Isosceles(pBeg, pEnd, wBeg * 2, wEnd * 2)
	        .points;

	canvas.circle(Geom::Circle(pBeg, wBeg));

	if (pBeg != pEnd) {
		canvas.circle(Geom::Circle(pEnd, wEnd));

		if (options.straightFactor == 1.0) {
			canvas.beginPolygon();
			canvas.addPoint(p0);
			canvas.addPoint(p1);
			canvas.addPoint(p2);
			canvas.addPoint(p3);
			canvas.endPolygon();
		}
		else {
			DrawPolygon({options.coordSys.getOriginal(p0),
			                options.coordSys.getOriginal(p1),
			                options.coordSys.getOriginal(p2),
			                options.coordSys.getOriginal(p3)},
			    {static_cast<PathSampler::Options>(options),
			        {.toCircleFactor = 0,
			            .straightFactor = options.straightFactor}},
			    canvas,
			    false);
		}
	}
}

DrawLine::Path::Path(const Geom::Point &p0,
    const Geom::Point &p1,
    Gfx::ICanvas &canvas,
    const Options &options) :
    PathSampler(p0, p1, options),
    canvas(canvas)
{}

void DrawLine::Path::sample()
{
	lastPoint = getPoint(0.0);
	calc();
	addPoint(getPoint(1.0));
}

void DrawLine::Path::addPoint(const Geom::Point &point)
{
	Geom::Line line;
	line.begin = lastPoint;
	line.end = point;
	canvas.line(line);
	lastPoint = point;
}

Geom::Point DrawLine::Path::getPoint(double i)
{
	return coordSys.convert(Math::interpolate<>(p0, p1, i));
}

}
