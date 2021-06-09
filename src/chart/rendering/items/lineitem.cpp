#include "lineitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

LineItem::LineItem(const Diag::Marker &item,
    const Diag::DiagramOptions &options,
    const Styles::Chart &style,
    const Diag::Diagram::Markers &items,
    size_t lineIndex) :
    ConnectingDrawItem(item,
        options,
        items,
        lineIndex,
        Diag::ShapeType::Line)
{
	minWidth = *style.data.lineMinWidth;
	width = *style.data.lineWidth;
	maxWidth = *style.data.lineMaxWidth;

	linear = true;
	center = Math::interpolate(
		item.position.yComp(),
	    item.position.xComp(),
	    (double)options.horizontal.get());

	auto spacing = item.spacing * item.size / 2;
	auto pos = item.position - spacing;

	if ((double)enabled > 0.0)
	{
		lineWidth[1] = getWidth(item.sizeFactor);

		const auto *prev = getPrev(item, items, lineIndex);
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
				points[1] = pos - item.size.yComp();
			}
			else
			{
				points[0] = prevPos - prev->size.xComp();
				points[1] = pos - item.size.xComp();
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
