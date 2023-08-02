#include "drawline.h"

#include "base/geom/quadrilateral.h"
#include "base/math/interpolation.h"

#include "drawpolygon.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Draw;

DrawLine::DrawLine(const Geom::Line &line,
    ResolutionMode resolutionMode,
    CoordinateSystem &coordSys,
    Gfx::ICanvas &canvas)
{
	PathSampler::Options options(coordSys);
	options.resolutionMode = resolutionMode;
	Path(line.begin, line.end, canvas, options).sample();
}

DrawLine::DrawLine(const Geom::Line &line,
    std::array<double, 2> widths,
    double straightFactor,
    const Gfx::Color &endColor,
    const Gfx::Color &lineColor,
    CoordinateSystem &coordSys,
    Gfx::ICanvas &canvas)
{
	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = widths[0] * coordSys.getRect().size.minSize();
	auto wEnd = widths[1] * coordSys.getRect().size.minSize();

	const auto &[p0, p1, p2, p3] =
	    ConvexQuad::Isosceles(pBeg, pEnd, wBeg * 2, wEnd * 2).points;

	canvas.setBrushColor(endColor);
	canvas.setLineColor(endColor);

	canvas.circle(Geom::Circle(pBeg, wBeg));

	if (pBeg != pEnd) {
		canvas.circle(Geom::Circle(pEnd, wEnd));

		canvas.setBrushColor(lineColor);
		canvas.setLineColor(lineColor);

		constexpr bool supportCurve = false;
		if constexpr (supportCurve) {
			if (straightFactor > 0) {
				Draw::DrawPolygon::Options options(coordSys);
				options.circ = 0;
				options.linear = straightFactor;
				auto ps = std::array<Geom::Point, 4>{
				    coordSys.getOriginal(p0),
				    coordSys.getOriginal(p1),
				    coordSys.getOriginal(p2),
				    coordSys.getOriginal(p3)};
				Draw::DrawPolygon(ps, options, canvas, false);
				return;
			}
		}
		canvas.beginPolygon();
		canvas.addPoint(p0);
		canvas.addPoint(p1);
		canvas.addPoint(p2);
		canvas.addPoint(p3);
		canvas.endPolygon();
	}
}

DrawLine::Path::Path(const Point &p0,
    const Point &p1,
    Gfx::ICanvas &canvas,
    const PathSampler::Options &options) :
    PathSampler(p0, p1, options),
    canvas(canvas)
{}

void DrawLine::Path::sample()
{
	lastPoint = getPoint(0.0);
	calc();
	addPoint(getPoint(1.0));
}

void DrawLine::Path::addPoint(const Point &point)
{
	Geom::Line line;
	line.begin = lastPoint;
	line.end = point;
	canvas.line(line);
	lastPoint = point;
}

Point DrawLine::Path::getPoint(double i)
{
	return drawOptions.coordSys.convert(
	    Math::interpolate<>(p0, p1, i));
}
