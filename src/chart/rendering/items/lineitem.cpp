#include "lineitem.h"

using namespace Vizzu;
using namespace Vizzu::Draw;

LineItem::LineItem(const Gen::Marker &marker,
    const CoordinateSystem &coordSys,
    const Gen::Options &options,
    const Styles::Chart &style,
    const Gen::Plot::Markers &markers,
    size_t lineIndex) :
    ConnectingDrawItem(marker,
        coordSys,
        options,
        markers,
        lineIndex,
        Gen::ShapeType::line)
{
	auto minWidth = *style.plot.marker.lineMinWidth;
	auto maxWidth = *style.plot.marker.lineMaxWidth;

	linear = true;
	center = Math::interpolate(marker.position.yComp(),
	    marker.position.xComp(),
	    static_cast<double>(options.horizontal));

	auto spacing = marker.spacing * marker.size / 2;
	auto pos = marker.position - spacing;

	if (static_cast<double>(labelEnabled) > 0.0) {
		lineWidth[1] =
		    std::max(maxWidth * marker.sizeFactor, minWidth);

		points[2] = pos;

		points[1] = pos
		          - (static_cast<double>(options.horizontal) > 0.5
		                  ? marker.size.yComp()
		                  : marker.size.xComp());

		const auto *prev = getPrev(marker, markers, lineIndex);
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

			lineWidth[0] =
			    std::max(maxWidth * prev->sizeFactor, minWidth);

			points[3] = prevPos;

			points[0] = prevPos
			          - (static_cast<double>(options.horizontal) > 0.5
			                  ? prev->size.yComp()
			                  : prev->size.xComp());

			center = pos;
		}
		else {
			center = points[3] = pos;
			points[0] = points[1];
			lineWidth[0] = lineWidth[1];
		}
	}
	else {
		center = Geom::Point(pos.x, 0);
	}
	dataRect.pos = points[2];
	dataRect.size = Geom::Size();
	radius = lineWidth[1] * coordSys.getRect().size.minSize();
}
