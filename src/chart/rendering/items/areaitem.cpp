#include "areaitem.h"

#include "base/geom/trapezoid.h"
#include "base/io/log.h"

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
	enabled = enabled && connected;
	linear = true;

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	lineWidth[0] = lineWidth[1] = 0;

	if ((double)labelEnabled > 0.0)
	{
		const auto *prev = getPrev(marker, markers, lineIndex);

		points[2] = pos;
		points[1] = pos - ((double)options.horizontal.get() > 0.5 
			? marker.size.yComp() : marker.size.xComp());

		if (prev)
		{
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position;

			if ((double)options.polar.get() > 0)
			{
				if ((double)options.horizontal.get() > 0.5)
				{
					if (prevPos.x >= 1) prevPos.x -= 1;
				}
				else
				{
					if (prevPos.y >= 1) prevPos.y -= 1;
				}
			}

			prevPos = prevPos - prevSpacing;

			points[3] = prevPos;

			points[0] = prevPos - ((double)options.horizontal.get() > 0.5
				? prev->size.yComp() : prev->size.xComp());

			center = Geom::Point(pos.x, 0);
		}
		else
		{
			center = points[3] = pos;
			points[0] = points[1];
		}
	}
	else
	{
		center = Geom::Point(pos.x, 0);
	}
	dataRect.pos = points[1];
	dataRect.size = points[2] - points[1];
	radius = 0;
}

bool AreaItem::bounds(const Geom::Point &p)
{
	if ((double)enabled == 0) return false;
	return Geom::VerticalTrapezoid(points).contains(p);
}
