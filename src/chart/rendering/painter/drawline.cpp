#include "drawline.h"

#include "base/math/interpolation.h"

using namespace Geom;
using namespace Vizzu;
using namespace Vizzu::Draw;

drawLine::drawLine(const Geom::Line &line,
			 	   ResolutionMode resolutionMode,
				   CoordinateSystem &coordSys,
				   Gfx::ICanvas &canvas)
{
	PathSampler::Options options(coordSys);
	options.resolutionMode = resolutionMode;
	Path(line.begin, line.end, canvas, options).sample();
}

drawLine::drawLine(const Geom::Line &line,
			 	   std::array<double, 2> widths,
				   const Gfx::Color &endColor,
				   const Gfx::Color &lineColor,
				   CoordinateSystem &coordSys,
				   Gfx::ICanvas &canvas)
{
	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = widths[0] * coordSys.getRect().size.minSize();
	auto wEnd = widths[1] * coordSys.getRect().size.minSize();

	auto dir = (pEnd - pBeg).normalized();

	auto p0 = pBeg + dir.normal(false) * wBeg;
	auto p1 = pBeg + dir.normal(true) * wBeg;
	auto p2 = pEnd + dir.normal(true) * wEnd;
	auto p3 = pEnd + dir.normal(false) * wEnd;

	canvas.setBrushColor(endColor);
	canvas.setLineColor(endColor);

	canvas.circle(Geom::Circle(pBeg, wBeg));

	if (pBeg != pEnd)
	{
		canvas.circle(Geom::Circle(pEnd, wEnd));

		canvas.setBrushColor(lineColor);
		canvas.setLineColor(lineColor);

		canvas.beginPolygon();
		canvas.addPoint(p0);
		canvas.addPoint(p1);
		canvas.addPoint(p2);
		canvas.addPoint(p3);
		canvas.endPolygon();
	}
}

drawLine::Path::Path(const Point &p0, const Point &p1,
					 Gfx::ICanvas &canvas,
					 const PathSampler::Options &options)
	: PathSampler(p0,p1,options),
	  canvas(canvas)
{}

void drawLine::Path::sample()
{
	lastPoint = getPoint(0.0);
	calc();
	addPoint(getPoint(1.0));
}

void drawLine::Path::addPoint(const Point &point)
{
	Geom::Line line;
	line.begin = lastPoint;
	line.end = point;
	canvas.line(line);
	lastPoint = point;
}

Point drawLine::Path::getPoint(double i)
{
	return drawOptions.coordSys.convert(Math::interpolate<>(p0, p1, i));
}
