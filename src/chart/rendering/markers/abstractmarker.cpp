// clang-format off
#include "base/math/arrayoperators.h" // NOLINT
// clang-format on

#include "abstractmarker.h"

#include "circlemarker.h"
#include "connectingmarker.h"
#include "rectanglemarker.h"

namespace Vizzu::Draw
{

AbstractMarker AbstractMarker::create(const Gen::Marker &marker,
    const Gen::Options &options,
    const Gen::ShapeType &shapeType,
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Gen::Plot::Markers &markers,
    size_t lineIndex)
{
	switch (shapeType) {
	case Gen::ShapeType::rectangle:
		return RectangleMarker(marker, coordSys, options, style);
	case Gen::ShapeType::circle:
		return CircleMarker(marker, coordSys, options, style);
	case Gen::ShapeType::area:
	case Gen::ShapeType::line:
		return ConnectingMarker(marker,
		    coordSys,
		    options,
		    style,
		    markers,
		    lineIndex,
		    shapeType);
	default: return {marker, coordSys, options};
	}
}

AbstractMarker AbstractMarker::createInterpolated(
    const Gen::Marker &marker,
    const Gen::Options &options,
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Gen::Plot::Markers &markers,
    size_t lineIndex)
{
	auto fromShapeType = options.geometry.get(0).value;

	auto fromMarker = create(marker,
	    options,
	    fromShapeType,
	    style,
	    coordSys,
	    markers,
	    lineIndex);

	auto toShapeType = options.geometry.get(1).value;

	if (fromShapeType == toShapeType) return fromMarker;

	auto toMarker = create(marker,
	    options,
	    toShapeType,
	    style,
	    coordSys,
	    markers,
	    lineIndex);

	AbstractMarker aMarker(marker, coordSys, options);
	aMarker.enabled = fromMarker.enabled + toMarker.enabled;
	aMarker.labelEnabled =
	    fromMarker.labelEnabled + toMarker.labelEnabled;

	auto sum =
	    static_cast<double>(fromMarker.enabled + toMarker.enabled);

	using Math::interpolate;
	if (sum > 0.0) {
		auto factor = static_cast<double>(toMarker.enabled) / sum;
		aMarker.morphToCircle = interpolate(fromMarker.morphToCircle,
		    toMarker.morphToCircle,
		    factor);
		aMarker.linear =
		    interpolate(fromMarker.linear, toMarker.linear, factor);
		aMarker.border =
		    interpolate(fromMarker.border, toMarker.border, factor);
		aMarker.points =
		    interpolate(fromMarker.points, toMarker.points, factor);
		aMarker.lineWidth = interpolate(fromMarker.lineWidth,
		    toMarker.lineWidth,
		    factor);
		aMarker.connected = interpolate(fromMarker.connected,
		    toMarker.connected,
		    factor);
		aMarker.center =
		    interpolate(fromMarker.center, toMarker.center, factor);
	}
	sum = static_cast<double>(
	    fromMarker.labelEnabled + toMarker.labelEnabled);
	if (sum > 0.0) {
		auto factor =
		    static_cast<double>(toMarker.labelEnabled) / sum;
		aMarker.dataRect = interpolate(fromMarker.dataRect,
		    toMarker.dataRect,
		    factor);
		aMarker.radius =
		    interpolate(fromMarker.radius, toMarker.radius, factor);
	}
	return aMarker;
}

Geom::Rect AbstractMarker::getBoundary() const
{
	return Geom::Rect::Boundary(points);
}

Geom::Line AbstractMarker::getLine() const
{
	return {points[3], points[2]};
}

Geom::Line AbstractMarker::getStick() const
{
	return {points[1], points[2]};
}

Geom::Line AbstractMarker::getLabelPos(
    Styles::MarkerLabel::Position position,
    const CoordinateSystem &coordSys) const
{
	Geom::Point center;
	Geom::Point direction;

	typedef Styles::MarkerLabel::Position Pos;
	switch (position) {
	default:
	case Pos::center:
	case Pos::top: direction = Geom::Point{0, 1}; break;
	case Pos::bottom: direction = Geom::Point{0, -1}; break;
	case Pos::left: direction = Geom::Point{-1, 0}; break;
	case Pos::right: direction = Geom::Point{1, 0}; break;
	}

	if (position == Pos::center) { center = dataRect.center(); }
	else {
		Geom::Line side;
		switch (position) {
		default:
		case Pos::top: side = dataRect.positive().topSide(); break;
		case Pos::bottom:
			side = dataRect.positive().bottomSide();
			break;
		case Pos::left: side = dataRect.positive().leftSide(); break;
		case Pos::right:
			side = dataRect.positive().rightSide();
			break;
		}
		center = side.center();
	}

	auto res =
	    coordSys.convertDirectionAt({center, center + direction});

	if (position != Pos::center)
		res.shift(res.getDirection() * radius);

	return res;
}

bool AbstractMarker::bounds(const Geom::Point &point)
{
	if (static_cast<double>(enabled) == 0) return false;

	/** Approximated solution */
	auto isInside = shapeType.combine<Math::FuzzyBool>(
	    [this, &point](int, const Gen::ShapeType &shapeType)
	    {
		    switch (shapeType) {
		    case Gen::ShapeType::rectangle:
		    case Gen::ShapeType::area:
			    return Math::FuzzyBool{
			        Geom::ConvexQuad(points).contains(point, 0.01)};
		    case Gen::ShapeType::circle:
			    return Math::FuzzyBool{
			        Geom::Circle(Geom::Rect::Boundary(points),
			            Geom::Circle::FromRect::sameWidth)
			            .overlaps(Geom::Circle(point, 0.01), 0.1)};
		    case Gen::ShapeType::line:
			    return Math::FuzzyBool{
			        lineToQuad(10.0).contains(coordSys.convert(point),
			            0.1)};
		    }
		    return Math::FuzzyBool{false};
	    });

	return isInside != false;
}

Geom::ConvexQuad AbstractMarker::lineToQuad(double atLeastWidth) const
{
	auto line = getLine();

	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = lineWidth[0] * coordSys.getRect().size.minSize();
	auto wEnd = lineWidth[1] * coordSys.getRect().size.minSize();
	return Geom::ConvexQuad::Isosceles(pBeg,
	    pEnd,
	    std::max(atLeastWidth, wBeg * 2),
	    std::max(atLeastWidth, wEnd * 2));
}

AbstractMarker::AbstractMarker(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options) :
    marker(marker),
    coordSys(coordSys),
    shapeType(options.geometry),
    enabled(false),
    labelEnabled(false)
{}

SingleDrawMarker::SingleDrawMarker(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    Gen::ShapeType type) :
    AbstractMarker(marker, coordSys, options)
{
	enabled = options.geometry.factor<Math::FuzzyBool>(type)
	       && marker.enabled;

	labelEnabled = enabled;
}

}