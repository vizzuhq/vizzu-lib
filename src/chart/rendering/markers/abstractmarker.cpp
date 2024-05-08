// clang-format off
#include "base/math/arrayoperators.h" // NOLINT
// clang-format on

#include "abstractmarker.h"

#include "circlemarker.h"
#include "connectingmarker.h"
#include "rectanglemarker.h"

namespace Vizzu::Draw
{

AbstractMarker AbstractMarker::create(const DrawingContext &ctx,
    const Gen::Marker &marker,
    const Gen::ShapeType &shapeType,
    size_t lineIndex)
{
	switch (shapeType) {
	case Gen::ShapeType::rectangle:
		return RectangleMarker(marker,
		    ctx.getOptions(),
		    ctx.rootStyle);
	case Gen::ShapeType::circle:
		return CircleMarker(marker,
		    ctx.coordSys,
		    ctx.getOptions(),
		    ctx.rootStyle);
	case Gen::ShapeType::area:
	case Gen::ShapeType::line:
		return ConnectingMarker(ctx, marker, lineIndex, shapeType);
	default: return {marker, ctx.getOptions()};
	}
}

AbstractMarker AbstractMarker::createInterpolated(
    const DrawingContext &ctx,
    const Gen::Marker &marker,
    size_t lineIndex)
{
	const auto &options = ctx.getOptions();

	auto fromShapeType = options.geometry.get(false).value;

	auto fromMarker = create(ctx, marker, fromShapeType, lineIndex);

	auto toShapeType = options.geometry.get(true).value;

	if (fromShapeType == toShapeType) return fromMarker;

	auto toMarker = create(ctx, marker, toShapeType, lineIndex);

	AbstractMarker aMarker(marker, options);
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

AbstractMarker::AbstractMarker(const Gen::Marker &marker,
    const Gen::Options &options) :
    marker(marker),
    shapeType(options.geometry),
    enabled(false),
    labelEnabled(false)
{}

SingleDrawMarker::SingleDrawMarker(const Gen::Marker &marker,
    const Gen::Options &options,
    Gen::ShapeType type) :
    AbstractMarker(marker, options)
{
	enabled = options.geometry.factor<Math::FuzzyBool>(type)
	       && marker.enabled;

	labelEnabled = enabled;
}

}