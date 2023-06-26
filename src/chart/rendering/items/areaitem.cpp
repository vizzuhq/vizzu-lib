#include "areaitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

AreaItem::AreaItem(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    const Gen::Plot::Markers &markers,
    size_t lineIndex) :
    ConnectingDrawItem(marker,
        coordSys,
        options,
        markers,
        lineIndex,
        Gen::ShapeType::area)
{
	enabled = enabled && connected;
	linear = true;

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	lineWidth[0] = lineWidth[1] = 0;

	if (static_cast<double>(labelEnabled) > 0.0) {
		const auto *prev = getPrev(marker, markers, lineIndex);

		points[2] = pos;
		points[1] = pos
		          - (static_cast<double>(options.horizontal) > 0.5
		                  ? marker.size.yComp()
		                  : marker.size.xComp());

		if (prev) {
			auto prevSpacing = prev->spacing * prev->size / 2;
			auto prevPos = prev->position;

			if (static_cast<double>(options.polar) > 0) {
				if (static_cast<double>(options.horizontal) > 0.5) {
					if (prevPos.x >= 1) prevPos.x -= 1;
				}
				else {
					if (prevPos.y >= 1) prevPos.y -= 1;
				}
			}

			prevPos = prevPos - prevSpacing;

			points[3] = prevPos;

			points[0] = prevPos
			          - (static_cast<double>(options.horizontal) > 0.5
			                  ? prev->size.yComp()
			                  : prev->size.xComp());

			center = Geom::Point(pos.x, 0);
		}
		else {
			center = points[3] = pos;
			points[0] = points[1];
		}
	}
	else {
		center = Geom::Point(pos.x, 0);
	}
	dataRect.pos = points[1];
	dataRect.size = points[2] - points[1];
	radius = 0;
}
