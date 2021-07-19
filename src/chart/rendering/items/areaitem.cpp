#include "areaitem.h"

#include "base/geom/trapezoid.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

AreaItem::AreaItem(const Diag::Marker &marker,
    const Diag::Options &options,
    const Diag::Diagram::Markers &markers,
    size_t lineIndex) :
    ConnectingDrawItem(marker,
        options,
        markers,
        lineIndex,
        Diag::ShapeType::Area)
{
	enabled = enabled * connected;
	linear = true;

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	lineWidth[0] = lineWidth[1] = 0;

	if ((double)enabled > 0.0)
	{
		const auto *prev = getPrev(marker, markers, lineIndex);

		if (prev)
		{
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position - prevSpacing;

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
			center = Geom::Point(pos.x, 0);
		}
		else
		{
			center = points[2] = points[3] = pos;
		}
	}
	else
	{
		center = Geom::Point(pos.x, 0);
	}
}

bool AreaItem::bounds(const Geom::Point &p)
{
	if ((double)enabled == 0) return false;
	return Geom::VerticalTrapezoid(points).contains(p);
}
