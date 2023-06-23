#include "base/math/arrayoperators.h"

#include "drawitem.h"

#include "areaitem.h"
#include "circleitem.h"
#include "lineitem.h"
#include "rectangleitem.h"

using namespace Vizzu;
using namespace Math;
using namespace Vizzu::Draw;

DrawItem DrawItem::create(const Gen::Marker &marker,
    const Gen::Options &options,
    const Gen::ShapeType &shapeType, 
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Gen::Plot::Markers &markers,
    size_t lineIndex)
{
	switch(shapeType)
	{
		case Gen::ShapeType::rectangle:
			return RectangleItem(marker, coordSys, options, style);
		case Gen::ShapeType::area:
			return AreaItem(marker, coordSys, options, markers, lineIndex);
		case Gen::ShapeType::line:
			return LineItem(marker, coordSys, options, style, markers, lineIndex);
		case Gen::ShapeType::circle:
			return CircleItem(marker, coordSys, options, style);
		default:
			return DrawItem(marker, coordSys, options);
	}
}

DrawItem DrawItem::createInterpolated(const Gen::Marker &marker,
    const Gen::Options &options,
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Gen::Plot::Markers &markers,
    size_t lineIndex) 
{
	auto fromShapeType = options.shapeType.get(0).value;

	auto fromItem = create(marker, options, fromShapeType, style, 
		coordSys, markers, lineIndex);

	auto toShapeType = options.shapeType.get(1).value;

	if (fromShapeType == toShapeType) return fromItem;

	auto toItem = create(marker, options, toShapeType, style, 
		coordSys, markers, lineIndex);

	DrawItem item(marker, coordSys, options, lineIndex);
	item.enabled = fromItem.enabled + toItem.enabled;
	item.labelEnabled = fromItem.labelEnabled + toItem.labelEnabled;

	auto sum = static_cast<double>(fromItem.enabled + toItem.enabled);
	if (sum > 0.0)
	{
		auto factor = static_cast<double>(toItem.enabled) / sum;
		item.morphToCircle = interpolate(fromItem.morphToCircle, toItem.morphToCircle, factor);
		item.linear = interpolate(fromItem.linear, toItem.linear, factor);
		item.border = interpolate(fromItem.border, toItem.border, factor);
		item.points = interpolate(fromItem.points, toItem.points, factor);
		item.lineWidth = interpolate(fromItem.lineWidth, toItem.lineWidth, factor);
		item.connected = interpolate(fromItem.connected, toItem.connected, factor);
		item.color = interpolate(fromItem.color, toItem.color, factor);
		item.center = interpolate(fromItem.center, toItem.center, factor);
	}
	sum = static_cast<double>(fromItem.labelEnabled + toItem.labelEnabled);
	if (sum > 0.0)
	{
		auto factor = static_cast<double>(toItem.labelEnabled) / sum;
		item.dataRect = interpolate(fromItem.dataRect, toItem.dataRect, factor);
		item.radius = interpolate(fromItem.radius, toItem.radius, factor);
	}
	return item;
}

Geom::Rect DrawItem::getBoundary() const
{
	return Geom::Rect::Boundary(points);
}

Geom::Line DrawItem::getLine() const
{
	return Geom::Line(points[3], points[2]);
}

Geom::Line DrawItem::getStick() const
{
	return Geom::Line(points[1], points[2]);
}

Geom::Line DrawItem::getLabelPos(
    Styles::MarkerLabel::Position position,
    const CoordinateSystem &coordSys) const
{
	Geom::Point center;
	Geom::Point direction;

	typedef Styles::MarkerLabel::Position Pos;
	switch (position) {
	default:
	case Pos::center:
	case Pos::top: direction = Geom::Point(0, 1); break;
	case Pos::bottom: direction = Geom::Point(0, -1); break;
	case Pos::left: direction = Geom::Point(-1, 0); break;
	case Pos::right: direction = Geom::Point(1, 0); break;
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

	auto res = coordSys.convertDirectionAt(
	    Geom::Line(center, center + direction));

	if (position != Pos::center)
		res.shift(res.getDirection() * radius);

	return Geom::Line(res);
}

bool DrawItem::bounds(const Geom::Point &point)
{
	if (static_cast<double>(enabled) == 0) return false;

	/** Approximated solution */
	auto isInside = shapeType.combine<Math::FuzzyBool>(
	[&, this](int, const Gen::ShapeType &shapeType) {
		return Math::FuzzyBool(
			shapeType == Gen::ShapeType::rectangle ||
			shapeType == Gen::ShapeType::area
			? Geom::ConvexQuad(points).contains(point, 0.001) :

			shapeType == Gen::ShapeType::line 
			? lineToQuad().contains(coordSys.convert(point), 0.1) :

			shapeType == Gen::ShapeType::circle
			? Geom::Circle(Geom::Rect::Boundary(points),
			    Geom::Circle::FromRect::sameWidth)
			    .contains(point) :

			false);
	});

	return isInside != false;
}

Geom::ConvexQuad DrawItem::lineToQuad() const
{
	auto line = getLine();

	auto pBeg = coordSys.convert(line.begin);
	auto pEnd = coordSys.convert(line.end);

	auto wBeg = lineWidth[0] * coordSys.getRect().size.minSize();
	auto wEnd = lineWidth[1] * coordSys.getRect().size.minSize();

	return Geom::ConvexQuad::Isosceles(pBeg, pEnd, wBeg * 2, wEnd * 2);
}

SingleDrawItem::SingleDrawItem(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    Gen::ShapeType type) :
    DrawItem(marker, coordSys, options)
{
	color = marker.color;
	enabled =
	    options.shapeType.factor<Math::FuzzyBool>(type) && marker.enabled;
	labelEnabled = enabled;
}
