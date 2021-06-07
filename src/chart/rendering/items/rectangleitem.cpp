#include "rectangleitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

RectangleItem::RectangleItem(const Diag::DiagramItem &item,
    const Diag::DiagramOptions &options,
    const Styles::Chart &style) :
    SingleDrawItem(item, options, Diag::ShapeType::Rectangle)
{
	linear = (double)options.polar.get() == 0;
	border = Math::FuzzyBool(true);

	auto maxSpacing = style.data.maxPadding();
	auto decrease = style.data.paddingDecrease();

	auto spacing = item.spacing * Geom::Point(
		item.size.x - std::max(0.0, maxSpacing.x*(1-exp(-item.size.x * decrease.x))),
		item.size.y - std::max(0.0, maxSpacing.y*(1-exp(-item.size.y * decrease.y)))) / 2;

	center = item.position - item.spacing * item.size / 2;

	points[0] = item.position - item.size + spacing;
	points[1] = item.position - item.size.yComp() + spacing.flipX();
	points[2] = item.position - spacing;
	points[3] = item.position - item.size.xComp() + spacing.flipY();
	lineWidth[0] = lineWidth[1] = 0;
}

bool RectangleItem::bounds(const Geom::Point &p)
{
	if (!enabled) return false;
	return Geom::Rect::Boundary(points).contains(p);
}
