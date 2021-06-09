#include "lineitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

LineItem::LineItem(const Diag::Marker &marker,
    const Diag::DiagramOptions &options,
    const Styles::Chart &style,
    const Diag::Diagram::Markers &markers,
    size_t lineIndex) :
    ConnectingDrawItem(marker,
        options,
        markers,
        lineIndex,
        Diag::ShapeType::Line)
{
	minWidth = *style.data.lineMinWidth;
	width = *style.data.lineWidth;
	maxWidth = *style.data.lineMaxWidth;

	linear = true;
	center = Math::interpolate(
		marker.position.yComp(),
	    marker.position.xComp(),
	    (double)options.horizontal.get());

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	if ((double)enabled > 0.0)
	{
		lineWidth[1] = getWidth(marker.sizeFactor);

		const auto *prev = getPrev(marker, markers, lineIndex);
		if (prev)
		{
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position - prevSpacing;

			lineWidth[0] = getWidth(prev->sizeFactor);

			points[3] = prevPos;
			points[2] = pos;

			if ((double)options.horizontal.get() > 0.5)
			{
				points[0] = prevPos - prev->size.yComp();
				points[1] = pos - marker.size.yComp();
			}
			else
			{
				points[0] = prevPos - prev->size.xComp();
				points[1] = pos - marker.size.xComp();
			}
			center = pos;
		}
		else
		{
			center = points[2] = points[3] = pos;
			lineWidth[0] = lineWidth[1];
		}
	}
	else
	{
		center = Geom::Point(pos.x, 0);
	}
}

bool LineItem::bounds(const Geom::Point &)
{
	if (!enabled) return false;
	return false; // todo: missing
//	return VerticalTrapezoid<Linear>(getLine()).contains(p);
}

double LineItem::getWidth(double sizeFactor) const
{
	return std::min(std::max(minWidth, width * sizeFactor), maxWidth);
}
