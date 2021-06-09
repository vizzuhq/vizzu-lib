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
    const Diag::Diagram::Markers &markers)
{
	if (options.shapeType.get() == Diag::ShapeType::Rectangle)
		return std::make_unique<RectangleItem>(marker, options, style);

	if (options.shapeType.get() == Diag::ShapeType::Area)
		return std::make_unique<AreaItem>(marker, options, markers, 0);

	if (options.shapeType.get() == Diag::ShapeType::Line)
		return std::make_unique<LineItem>(marker, options, style, markers, 0);

	if (options.shapeType.get() == Diag::ShapeType::Circle)
		return std::make_unique<CircleItem>(marker, options, style);

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

SingleDrawItem::SingleDrawItem(const Diag::Marker &marker,
    const Diag::Options &options,
    Diag::ShapeType::Type type)
{
	color = marker.color;
	enabled = options.shapeType.get().getFactor(type) * marker.enabled;
	connected = 0;
}
