#include "base/math/arrayoperators.h"

#include "drawitem.h"

#include "base/io/log.h"

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
	if (shapeType == Gen::ShapeType::rectangle)
		return RectangleItem(marker, options, style);
	if (shapeType == Gen::ShapeType::area)
		return AreaItem(marker, options, markers, lineIndex);
	if (shapeType == Gen::ShapeType::line)
		return LineItem(marker, options, style, coordSys, markers, lineIndex);
	if (shapeType == Gen::ShapeType::circle)
		return CircleItem(marker, options, style, coordSys);
	return DrawItem(marker);
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

	DrawItem item(marker, lineIndex);
	item.enabled = fromItem.enabled + toItem.enabled;
	item.labelEnabled = fromItem.labelEnabled + toItem.labelEnabled;

	auto sum = (double)fromItem.enabled + (double)toItem.enabled;
	if (sum > 0.0)
	{
		auto factor = (double)toItem.enabled / sum;
		item.morphToCircle = interpolate(fromItem.morphToCircle, toItem.morphToCircle, factor);
		item.linear = interpolate(fromItem.linear, toItem.linear, factor);
		item.border = interpolate(fromItem.border, toItem.border, factor);
		item.points = interpolate(fromItem.points, toItem.points, factor);
		item.lineWidth = interpolate(fromItem.lineWidth, toItem.lineWidth, factor);
		item.connected = interpolate(fromItem.connected, toItem.connected, factor);
		item.color = interpolate(fromItem.color, toItem.color, factor);
		item.center = interpolate(fromItem.center, toItem.center, factor);
	}
	sum = (double)fromItem.labelEnabled + (double)toItem.labelEnabled;
	if (sum > 0.0)
	{
		auto factor = (double)toItem.labelEnabled / sum;
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

SingleDrawItem::SingleDrawItem(const Gen::Marker &marker,
    const Gen::Options &options,
    Gen::ShapeType type) :
    DrawItem(marker)
{
	color = marker.color;
	enabled =
	    options.shapeType.factor<Math::FuzzyBool>(type) && marker.enabled;
	labelEnabled = enabled;
}
