#include "drawpolygon.h"

#include "base/geom/rect.h"
#include "base/geom/triangle.h"
#include "base/math/interpolation.h"
#include "base/math/statistics.h"

using namespace Geom;
using namespace Gfx;
using namespace Vizzu;
using namespace Vizzu::Draw;

DrawPolygon::DrawPolygon(const std::array<Point, 4> &ps,
    const Options &options,
    ICanvas &canvas,
    bool clip)
{
	center = Math::mean(ps);
	boundary = Rect::Boundary(ps).size;

	auto linSize = Size{options.coordSys.verConvert(boundary.x),
	    options.coordSys.verConvert(boundary.y)};

	if (options.circ == 1.0 && linSize.isSquare(0.005)) {
		auto centerConv = options.coordSys.convert(center);
		auto radius = fabs(linSize.x) / 2.0;
		const Geom::Circle circle(centerConv, radius);
		if (clip)
			canvas.setClipCircle(circle);
		else
			canvas.circle(circle);
	}
	else {
		canvas.beginPolygon();

		Path(ps[0], ps[1], center, linSize, canvas, options).calc();
		Path(ps[1], ps[2], center, linSize, canvas, options).calc();
		Path(ps[2], ps[3], center, linSize, canvas, options).calc();
		Path(ps[3], ps[0], center, linSize, canvas, options).calc();

		if (clip)
			canvas.setClipPolygon();
		else
			canvas.endPolygon();
	}
}

DrawPolygon::Path::Path(const Point &p0,
    const Point &p1,
    Point center,
    Point linSize,
    ICanvas &canvas,
    const DrawPolygon::Options &options) :
    PathSampler(p0, p1, options),
    options(options),
    canvas(canvas),
    linSize(linSize)
{
	linP0 = options.coordSys.convert(p0);
	linP1 = options.coordSys.convert(p1);
	centerConv = options.coordSys.convert(center);
}

void DrawPolygon::Path::addPoint(const Point &point)
{
	canvas.addPoint(point);
}

Point DrawPolygon::Path::getPoint(double f)
{
	auto linP = Math::interpolate(linP0, linP1, f);
	auto nonlinP =
	    options.coordSys.convert(Math::interpolate(p0, p1, f));

	auto mixedP = Math::interpolate(nonlinP, linP, options.linear);

	return intpToElipse(mixedP, options.circ);
}

Point DrawPolygon::Path::intpToElipse(Point point, double factor)
{
	auto projected = projectToElipse(point);
	return Math::interpolate(point, projected, factor);
}

Point DrawPolygon::Path::projectToElipse(Point point)
{
	auto fi = (point - centerConv).angle();
	return centerConv + Point::Polar(1, fi) * linSize / 2.0;
}
