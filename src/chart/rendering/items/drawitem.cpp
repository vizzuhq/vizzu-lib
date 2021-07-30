#include "drawitem.h"

#include "areaitem.h"
#include "circleitem.h"
#include "lineitem.h"
#include "rectangleitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

std::unique_ptr<DrawItem> DrawItem::create(const Diag::Marker &marker,
    const Diag::Options &options,
    const Styles::Chart &style,
    const CoordinateSystem &coordSys,
    const Diag::Diagram::Markers &markers)
{
	if (options.shapeType.get() == Diag::ShapeType::Rectangle)
		return std::make_unique<RectangleItem>(marker, options, style);

	if (options.shapeType.get() == Diag::ShapeType::Area)
		return std::make_unique<AreaItem>(marker, options, markers, 0);

	if (options.shapeType.get() == Diag::ShapeType::Line)
		return std::make_unique<LineItem>
		(marker, options, style, coordSys, markers, 0);

	if (options.shapeType.get() == Diag::ShapeType::Circle)
		return std::make_unique<CircleItem>(marker, options, style, coordSys);

	throw std::logic_error("no shape set in options, cannot create draw marker");
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

Geom::Line DrawItem::getLabelPos(Styles::MarkerLabel::Position position,
	const CoordinateSystem &coordSys) const
{
	const auto small = .00000000001;

	Geom::Point center;
	Geom::Point direction;

	typedef Styles::MarkerLabel::Position Pos;
	switch (position)
	{
		default:
		case Pos::center:
		case Pos::top: direction = Geom::Point(0,small); break;
		case Pos::bottom: direction = Geom::Point(0,-small); break;
		case Pos::left: direction = Geom::Point(-small,0); break;
		case Pos::right: direction = Geom::Point(small,0); break;
	}

	if (position == Pos::center)
	{
		center = dataRect.center();
	}
	else
	{
		Geom::Line side;
		switch (position)
		{
			default:
			case Pos::top: side = dataRect.positive().topSide(); break;
			case Pos::bottom: side = dataRect.positive().bottomSide(); break;
			case Pos::left: side = dataRect.positive().leftSide(); break;
			case Pos::right: side = dataRect.positive().rightSide(); break;
		}
		center = side.center();
	}

	auto centerC = coordSys.convert(center);

	auto end = center + direction;
	auto endC = coordSys.convert(end);

	auto directionC = (endC - centerC).normalized();

	if (position != Pos::center) centerC = centerC + directionC * radius;

	endC = centerC + directionC;

	return Geom::Line(centerC, endC);
}


SingleDrawItem::SingleDrawItem(const Diag::Marker &marker,
    const Diag::Options &options,
    Diag::ShapeType::Type type)
{
	color = marker.color;
	enabled = options.shapeType.get().getFactor(type) * marker.enabled;
	connected = 0;
}
