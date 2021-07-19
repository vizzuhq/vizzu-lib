#include "rectangleitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

RectangleItem::RectangleItem(const Diag::Marker &marker,
    const Diag::Options &options,
    const Styles::Chart &style) :
    SingleDrawItem(marker, options, Diag::ShapeType::Rectangle)
{
	linear = (double)options.polar.get() == 0;
	border = Math::FuzzyBool(true);

	auto maxSpacing = style.data.maxPadding();
	auto decrease = style.data.paddingDecrease();

	auto spacing = marker.spacing * Geom::Point(
		marker.size.x - std::max(0.0, maxSpacing.x*(1-exp(-marker.size.x * decrease.x))),
		marker.size.y - std::max(0.0, maxSpacing.y*(1-exp(-marker.size.y * decrease.y)))) / 2;

	center = marker.position - marker.spacing * marker.size / 2;

	points[0] = marker.position - marker.size + spacing;
	points[1] = marker.position - marker.size.yComp() + spacing.flipX();
	points[2] = marker.position - spacing;
	points[3] = marker.position - marker.size.xComp() + spacing.flipY();
	lineWidth[0] = lineWidth[1] = 0;
}

bool RectangleItem::bounds(const Geom::Point &p)
{
	if ((double)enabled == 0) return false;
	return Geom::Rect::Boundary(points).contains(p);
}
