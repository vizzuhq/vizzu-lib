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

	Geom::Size spacing = marker.spacing *
		style.plot.marker.rectangleSpacing->combine<Geom::Size>(
		[&](int, const auto rectangleSpacing)
		{
			if (rectangleSpacing) 
			{
				auto padding = *rectangleSpacing;
				auto spacing = padding / (2 * (padding + 1));
				return marker.size * Geom::Size::Square(spacing);
			}
			else 
			{
				auto minWidth = 0.2;
				auto decrease = 5.0;
				auto minPadding = 0.15;

				auto spacing = Geom::Point(
					marker.size.x - std::max(0.0, minWidth*(1-exp(-marker.size.x * decrease))),
					marker.size.y - std::max(0.0, minWidth*(1-exp(-marker.size.y * decrease)))
				) / 2.0;

				if (spacing.x < marker.size.x * minPadding)
					spacing.x = marker.size.x * minPadding;
				if (spacing.y < marker.size.y * minPadding)
					spacing.y = marker.size.y * minPadding;

				return spacing;
			}
		}
	);

	center = marker.position - marker.spacing * marker.size / 2;

	points[0] = marker.position - marker.size + spacing;
	points[1] = marker.position - marker.size.yComp() + spacing.flipX();
	points[2] = marker.position - spacing;
	points[3] = marker.position - marker.size.xComp() + spacing.flipY();
	lineWidth[0] = lineWidth[1] = 0;

	dataRect.pos = points[0];
	dataRect.size = points[2] - points[0];
	radius = 0;
}

bool RectangleItem::bounds(const Geom::Point &p)
{
	if ((double)enabled == 0) return false;
	return Geom::Rect::Boundary(points).contains(p);
}
